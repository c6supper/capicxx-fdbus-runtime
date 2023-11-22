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

#include <Extension/fdbus/constants.hpp>

#include <CommonAPI/CallInfo.hpp>
#include <CommonAPI/Types.hpp>
#include <CommonAPI/FDBus/Types.hpp>

namespace CommonAPI {
namespace FDBus {

const method_id_t ANY_METHOD = Extension::fdbus::ANY_METHOD;
const major_version_t ANY_MAJOR_VERSION = Extension::fdbus::ANY_MAJOR;
const minor_version_t ANY_MINOR_VERSION = Extension::fdbus::ANY_MINOR;

const major_version_t DEFAULT_MAJOR_VERSION = Extension::fdbus::DEFAULT_MAJOR;
const minor_version_t DEFAULT_MINOR_VERSION = Extension::fdbus::DEFAULT_MINOR;

const service_id_t MIN_SERVICE_ID = 0x0001;
const service_id_t MAX_SERVICE_ID = 0xFFFD;

const instance_id_t MIN_INSTANCE_ID = 0x0001;
const instance_id_t MAX_INSTANCE_ID = 0xFFFE;

const ms_t ASYNC_MESSAGE_REPLY_TIMEOUT_MS = 5000;
const ms_t ASYNC_MESSAGE_CLEANUP_INTERVAL_MS = 1000;

const method_id_t  MIN_METHOD_ID = 0x0001;
const method_id_t  MAX_METHOD_ID = 0xFFFE;

const eventgroup_id_t MIN_EVENTGROUP_ID = 0x0001;
const eventgroup_id_t MAX_EVENTGROUP_ID = 0xFFFE;

static const CommonAPI::CallInfo defaultCallInfo(CommonAPI::DEFAULT_SEND_TIMEOUT_MS);

} // namespace FDBus
} // namespace CommonAPI

#endif // COMMONAPI_FDBUS_CONSTANTS_HPP_
