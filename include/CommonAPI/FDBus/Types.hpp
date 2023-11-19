// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_FDBUS_TYPES_HPP_
#define COMMONAPI_FDBUS_TYPES_HPP_

#include <cstdint>
#include <string>

namespace CommonAPI {
namespace FDBus {

class Proxy;

typedef int64_t ms_t;

typedef std::uint32_t session_id_fake_t;

class Message;
typedef std::function<bool (const Message &) > MessageHandler_t;

} // namespace FDBus
} // namespace CommonAPI

#endif // COMMONAPI_FDBUS_TYPES_HPP_
