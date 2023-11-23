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
#include <functional>
#include <memory>

#include <Extension/fdbus/constants.hpp>

namespace CommonAPI {
namespace FDBus {

class Proxy;

typedef Extension::FDBus::service_t service_id_t;
typedef Extension::FDBus::method_t method_id_t;
typedef Extension::FDBus::event_t event_id_t;
typedef Extension::FDBus::instance_t instance_id_t;
typedef Extension::FDBus::eventgroup_t eventgroup_id_t;
typedef Extension::FDBus::session_t session_id_t;
typedef Extension::FDBus::client_t client_id_t;

typedef Extension::FDBus::byte_t byte_t;
typedef Extension::FDBus::length_t message_length_t;

typedef Extension::FDBus::return_code_e return_code_e;
typedef Extension::FDBus::message_type_e message_type_e;
typedef Extension::FDBus::state_type_e state_type_e;

typedef Extension::FDBus::major_version_t major_version_t;
typedef Extension::FDBus::minor_version_t minor_version_t;

typedef int64_t ms_t;

typedef uint32_t AvailabilityHandlerId_t;
typedef std::function<void(std::shared_ptr<Proxy>, service_id_t, instance_id_t, bool, void*)> AvailabilityHandler_t;
typedef std::function<bool(client_id_t, bool) > SubsciptionHandler_t;

typedef std::uint32_t session_id_fake_t;

class Message;
typedef std::function<bool(const Message&) > MessageHandler_t;

} // namespace FDBus
} // namespace CommonAPI

#endif // COMMONAPI_FDBUS_TYPES_HPP_
