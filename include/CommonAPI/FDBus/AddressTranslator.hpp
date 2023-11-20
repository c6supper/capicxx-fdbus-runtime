// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_FDBUS_ADDRESSTRANSLATOR_HPP_
#define COMMONAPI_FDBUS_ADDRESSTRANSLATOR_HPP_

#include <map>
#include <memory>
#include <mutex>

#include <CommonAPI/Export.hpp>
#include <CommonAPI/Address.hpp>
#include <CommonAPI/FDBus/Address.hpp>

namespace CommonAPI {
namespace FDBus {

class AddressTranslator {
public:

    COMMONAPI_EXPORT static std::shared_ptr<AddressTranslator> get();

    COMMONAPI_EXPORT AddressTranslator();

    COMMONAPI_EXPORT ~AddressTranslator();

    COMMONAPI_EXPORT void init();

    COMMONAPI_EXPORT bool translate(const std::string& _key, Address& _value);
    COMMONAPI_EXPORT bool translate(const CommonAPI::Address& _key, Address& _value);

    COMMONAPI_EXPORT bool translate(const Address& _key, std::string& _value);
    COMMONAPI_EXPORT bool translate(const Address& _key, CommonAPI::Address& _value);

    COMMONAPI_EXPORT void insert(const std::string& _address, const std::string& _service);

    COMMONAPI_EXPORT const Address& getAddressAlias(const Address& _address) const;

private:
    COMMONAPI_EXPORT bool readConfiguration();
    COMMONAPI_EXPORT void readServiceAlias(const std::string& _source,
        const std::string& _target);
    COMMONAPI_EXPORT bool readValue(const std::string& _data, Address& _sourceAddress);

    COMMONAPI_EXPORT bool isValidService(const std::string& _source) const;

private:
    std::string defaultConfig_;

    std::map<CommonAPI::Address, Address> forwards_;
    std::map<Address, CommonAPI::Address> backwards_;

    typedef std::tuple<Address> Alias_t;
    std::map<Address, Alias_t > aliases_;

    std::mutex mutex_;
};

} // namespace FDBus
} // namespace CommonAPI

#endif // COMMONAPI_FDBUS_ADDRESSTRANSLATOR_HPP_
