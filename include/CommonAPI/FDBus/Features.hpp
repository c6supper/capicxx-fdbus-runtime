// Copyright (C) 2023-2024, Calvin Ke, All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_FDBUS_FEATURES_HPP_
#define COMMONAPI_FDBUS_FEATURES_HPP_

#include <cstdint>
#include <string>

#include <fdbus/common_defs.h>

#define FDBUS_VERSION (FDB_VERSION_MAJOR * 10000 \
                     + FDB_VERSION_MINOR * 100 \
                     + FDB_VERSION_BUILD)

// 50400  greatest supported version
// 40300  least supported version

#endif // COMMONAPI_FDBUS_FEATURES_HPP_
