// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_FDBUS_CONSTANTS_HPP_
#define COMMONAPI_FDBUS_CONSTANTS_HPP_

#include <cstdint>
#include <string>

#include <fdbus/common_defs.h>

#include <CommonAPI/CallInfo.hpp>
#include <CommonAPI/Types.hpp>
#include <CommonAPI/FDBus/Types.hpp>

namespace CommonAPI {
namespace FDBus {

static const CommonAPI::CallInfo defaultCallInfo(CommonAPI::DEFAULT_SEND_TIMEOUT_MS);

} // namespace FDBus
} // namespace CommonAPI

#endif // COMMONAPI_FDBUS_CONSTANTS_HPP_
