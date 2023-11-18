// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined(COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_FDBUS_ADDRESS_HPP_
#define COMMONAPI_FDBUS_ADDRESS_HPP_

#include <iostream>

#include <CommonAPI/Export.hpp>
#include <CommonAPI/FDBus/Constants.hpp>
#include <CommonAPI/FDBus/Types.hpp>

namespace CommonAPI {
namespace FDBus {

class COMMONAPI_EXPORT Address {
public:
    Address() = delete;
    Address(const std::string& _service);
    Address(const Address& _source);

    Address& operator=(const Address& _source);

    bool operator==(const Address& _other) const;
    bool operator!=(const Address& _other) const;

    const std::string& getService() const;
    void setService(const std::string& _service);

private:
    std::string service_;

    friend std::ostream& operator<<(std::ostream& _out, const Address& _address);
};

} // namespace FDBus
} // namespace CommonAPI

#endif // COMMONAPI_FDBUS_ADDRESS_HPP_
