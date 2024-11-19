// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// ZPP file
///
/// \file   zpp/file.hpp
/// \author Vincent Hamp
/// \date   17/11/2024

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace zpp {

/// ZPP file
struct File {
  std::string id;
  uint16_t version{};
  std::vector<uint8_t> flash;
  std::vector<std::pair<uint16_t, uint8_t>> cvs;
  bool coded{};
  std::array<uint8_t, 4uz> load_code{};
};

}  // namespace zpp