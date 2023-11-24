// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef FDBUS_MESSAGE_HPP
#define FDBUS_MESSAGE_HPP

#include <memory>

#include <CommonAPI/Export.hpp>
#include <CommonAPI/FDBus/Types.hpp>

namespace Extension {
namespace FDBus {

class message {
public:
  virtual ~message() {}

  std::shared_ptr<payload> get_payload() const = 0;
  void set_payload(std::shared_ptr<payload> _payload) = 0;

  COMMONAPI_EXPORT virtual service_t get_service() const = 0;
  COMMONAPI_EXPORT virtual void set_service(service_t _service) = 0;
  COMMONAPI_EXPORT virtual instance_t get_instance() const = 0;
  COMMONAPI_EXPORT virtual void set_instance(instance_t _instance) = 0;
  COMMONAPI_EXPORT virtual method_t get_method() const = 0;
  COMMONAPI_EXPORT virtual void set_method(method_t _method) = 0;
  COMMONAPI_EXPORT virtual client_t get_client() const = 0;
  COMMONAPI_EXPORT virtual void set_client(client_t _client) = 0;
  COMMONAPI_EXPORT virtual session_t get_session() const = 0;
  COMMONAPI_EXPORT virtual void set_session(session_t _session) = 0;
  COMMONAPI_EXPORT virtual message_type_e get_message_type() const = 0;
  COMMONAPI_EXPORT virtual void set_message_type(message_type_e _type) = 0;
  COMMONAPI_EXPORT virtual return_code_e get_return_code() const = 0;
  COMMONAPI_EXPORT virtual void set_return_code(return_code_e _code) = 0;
  COMMONAPI_EXPORT virtual bool is_initial() const = 0;
  COMMONAPI_EXPORT virtual void set_initial(bool _is_initial) = 0;
  COMMONAPI_EXPORT virtual bool is_valid_crc() const = 0;
  COMMONAPI_EXPORT virtual void set_is_valid_crc(bool _is_valid_crc) = 0;

  COMMONAPI_EXPORT virtual bool serialize(serializer* _to) const = 0;
  COMMONAPI_EXPORT virtual bool deserialize(deserializer* _from) = 0;
};


} // namespace FDBus
} // namespace Extension

#endif // FDBUS_MESSAGE_HPP