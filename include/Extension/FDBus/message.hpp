// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef FDBUS_MESSAGE_HPP
#define FDBUS_MESSAGE_HPP

namespace Extension {
namespace FDBus {

class message : public message_base {
public:
  virtual ~message() {}

  std::shared_ptr<payload> get_payload() const = 0;

  void set_payload(std::shared_ptr<payload> _payload) = 0;
};


} // namespace FDBus
} // namespace Extension

#endif // FDBUS_MESSAGE_HPP