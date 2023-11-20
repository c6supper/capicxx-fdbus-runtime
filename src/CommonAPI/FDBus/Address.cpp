// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <CommonAPI/FDBus/Address.hpp>
#include <iomanip>

namespace CommonAPI {
namespace FDBus {

const std::string Address::INVALID_SERVICE = "invalid_service";

Address::Address()
    : service_(Address::INVALID_SERVICE) {
}

Address::Address(const std::string& _service)
    : service_(_service) {
}

bool Address::IsValid() {
    return (service_ != Address::INVALID_SERVICE);
}

Address::Address(const Address& _source)
    : service_(_source.service_) {
}

Address&
Address::operator=(const Address& _source) {
    service_ = _source.service_;

    return (*this);
}

bool
Address::operator==(const Address& _other) const {
    return (service_ == _other.service_);
}

bool
Address::operator!=(const Address& _other) const {
    return (service_ != _other.service_);
}

bool
Address::operator<(const Address& _other) const {
    return (service_ < _other.service_);
}

const std::string&
Address::getService() const {
    return service_;
}

void
Address::setService(const std::string& _service) {
    service_ = _service;
}

std::ostream&
operator<<(std::ostream& _out, const Address& _address) {
    _out << "["
        << _address.service_
        << "]";
    return (_out);
}

} // namespace FDBus
} // namespace CommonAPI
