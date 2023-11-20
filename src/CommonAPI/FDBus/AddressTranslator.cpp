// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <sys/stat.h>

#include <algorithm>
#include <sstream>

#include <CommonAPI/IniFileReader.hpp>
#include <CommonAPI/Logger.hpp>
#include <CommonAPI/FDBus/AddressTranslator.hpp>
#include <CommonAPI/FDBus/Constants.hpp>

namespace CommonAPI {
namespace FDBus {

const char* COMMONAPI_FDBUS_DEFAULT_CONFIG_FILE = "commonapi-fdbus.ini";
const char* COMMONAPI_FDBUS_DEFAULT_CONFIG_FOLDER = "/etc/";

#ifdef _WIN32
CRITICAL_SECTION critSec;
#endif

std::shared_ptr<AddressTranslator> AddressTranslator::get() {
    static std::shared_ptr<AddressTranslator> theTranslator
        = std::make_shared<AddressTranslator>();
    return theTranslator;
}

AddressTranslator::AddressTranslator() {
    init();
}

void
AddressTranslator::init() {
#ifdef _WIN32
    InitializeCriticalSection(&critSec);
#endif
    // Determine default configuration file
    const char* config = getenv("COMMONAPI_FDBUS_CONFIG");
    if (config) {
        defaultConfig_ = config;
        struct stat s;
        if (stat(defaultConfig_.c_str(), &s) != 0) {
            COMMONAPI_ERROR("Failed to load ini file passed via "
                    "COMMONAPI_FDBUS_CONFIG environment: ", defaultConfig_);
        }
    }
    else {
        defaultConfig_ = COMMONAPI_FDBUS_DEFAULT_CONFIG_FOLDER;
        defaultConfig_ += "/";
        defaultConfig_ += COMMONAPI_FDBUS_DEFAULT_CONFIG_FILE;
    }

    (void)readConfiguration();
}

AddressTranslator::~AddressTranslator() {
#ifdef _WIN32
    DeleteCriticalSection(&critSec);
#endif
}

bool
AddressTranslator::translate(const std::string& _key, Address& _value) {
    return translate(CommonAPI::Address(_key), _value);
}

bool
AddressTranslator::translate(const CommonAPI::Address& _key, Address& _value) {
    bool result(true);
#ifdef _WIN32
    EnterCriticalSection(&critSec);
#else
    std::lock_guard<std::mutex> itsLock(mutex_);
#endif
    const auto it = forwards_.find(_key);
    if (it != forwards_.end()) {
        _value = it->second;
    }
    else {
        COMMONAPI_ERROR(
            "Cannot determine SOME/IP address data for "
            "CommonAPI address \"", _key, "\"");
        result = false;
    }
#ifdef _WIN32
    LeaveCriticalSection(&critSec);
#endif
    return result;
}

bool
AddressTranslator::translate(const Address& _key, std::string& _value) {
    CommonAPI::Address address;
    bool result = translate(_key, address);
    _value = address.getAddress();
    return result;
}

bool
AddressTranslator::translate(const Address& _key, CommonAPI::Address& _value) {
    bool result(true);
#ifdef _WIN32
    EnterCriticalSection(&critSec);
#else
    std::lock_guard<std::mutex> itsLock(mutex_);
#endif
    const auto it = backwards_.find(_key);
    if (it != backwards_.end()) {
        _value = it->second;
    }
    else {
        COMMONAPI_ERROR(
            "Cannot determine CommonAPI address data for "
            "SOME/IP address \"", _key, "\"");
        result = false;
    }
#ifdef _WIN32
    LeaveCriticalSection(&critSec);
#endif
    return result;
}

void
AddressTranslator::insert(const std::string& _address, const std::string& _service) {
    if (isValidService(_service)) {
        CommonAPI::Address address(_address);
        Address fdbusAddress(_service);
#ifdef _WIN32
        EnterCriticalSection(&critSec);
#else
        std::lock_guard<std::mutex> itsLock(mutex_);
#endif
        auto fw = forwards_.find(address);
        auto bw = backwards_.find(fdbusAddress);
        if (fw == forwards_.end() && bw == backwards_.end()) {
            forwards_[address] = fdbusAddress;
            backwards_[fdbusAddress] = address;
            COMMONAPI_DEBUG(
                "Added address mapping: ", address, " <--> ", fdbusAddress);
        }
        else if (bw != backwards_.end() && bw->second != _address) {
            COMMONAPI_ERROR("Trying to overwrite existing FDBus address which is "
                    "already mapped to a CommonAPI address: ",
                    fdbusAddress, " <--> ", _address);
        }
        else if (fw != forwards_.end() && fw->second != fdbusAddress) {
            COMMONAPI_ERROR("Trying to overwrite existing CommonAPI address which is "
                    "already mapped to a FDBus address: ",
                    _address, " <--> ", fdbusAddress);
        }
#ifdef _WIN32
        LeaveCriticalSection(&critSec);
#endif
    }
}

bool
AddressTranslator::readConfiguration() {
#define MAX_PATH_LEN 255
    std::string config;
    bool tryLoadConfig(true);
    char currentDirectory[MAX_PATH_LEN];
#ifdef _WIN32
    if (GetCurrentDirectory(MAX_PATH_LEN, currentDirectory)) {
#else
    if (getcwd(currentDirectory, MAX_PATH_LEN)) {
#endif
        config = currentDirectory;
        config += "/";
        config += COMMONAPI_FDBUS_DEFAULT_CONFIG_FILE;

        struct stat s;
        if (stat(config.c_str(), &s) != 0) {
            config = defaultConfig_;
            if (stat(config.c_str(), &s) != 0) {
                tryLoadConfig = false;
            }
        }
    }

    IniFileReader reader;
    if (tryLoadConfig && !reader.load(config))
        return false;

    for (auto itsSection : reader.getSections()) {
        if (itsSection.first == "aliases") {
            for (auto itsMapping : itsSection.second->getMappings()) {
                if (itsMapping.first.find("service:") == 0) {
                    readServiceAlias(itsMapping.first.substr(sizeof("service:" - 1)), itsMapping.second);
                }
            }
        }
        else {
            std::string service = itsSection.second->getValue("service");

            insert(itsSection.first, service);
        }
    }

    return true;
}

bool
AddressTranslator::isValidService(const std::string & _service) const {
    if (_service.empty()) {
        COMMONAPI_ERROR(
            "Found invalid service identifier (", _service, ")");
        return false;
    }

    return true;
}

const Address&
AddressTranslator::getAddressAlias(const Address & _address) const {
    auto foundAddress = aliases_.find(_address);
    if (foundAddress != aliases_.end())
        return std::get<0>(foundAddress->second);
    return _address;
}

void
AddressTranslator::readServiceAlias(const std::string & _source, const std::string & _target) {
    Address itsSourceAddress, itsTargetAddress;

    if (readValue(_source, itsSourceAddress) &&
            readValue(_target, itsTargetAddress)) {

        auto findService = aliases_.find(itsSourceAddress);
        if (findService == aliases_.end()) {
            Alias_t itsTarget = std::make_tuple(itsTargetAddress);
            aliases_.insert(std::make_pair(itsSourceAddress, itsTarget));
        }
        else {
            if (itsTargetAddress != std::get<0>(findService->second)) {
                COMMONAPI_ERROR("Found multiple aliases for address ", itsSourceAddress);
            }
        }
    }
}

bool
AddressTranslator::readValue(const std::string & _data, Address & _address) {

    std::string itsService;

    auto foundService = _data.find(':');
    if (foundService == std::string::npos) {
        return false;
    }

    itsService = _data.substr(0, foundService);

    _address.setService(itsService);

    return true;
}

} /* namespace FDBus */
} /* namespace CommonAPI */
