// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Read ZPP file
///
/// \file   read.cpp
/// \author Vincent Hamp
/// \date   17/11/2024

#include <algorithm>
#include <cassert>
#include <charconv>
#include <fstream>
#include <ranges>
#include "zpp.hpp"

namespace zpp {

File read(std::filesystem::path path) {
  if (!exists(path))
    throw std::filesystem::filesystem_error(
      "ZPP file does not exist",
      path,
      std::make_error_code(std::errc::file_exists));

  // Read entire file to temporary buffer
  std::ifstream fr{path, std::ios::binary};
  std::vector<char> chunk(file_size(path));
  fr.read(data(chunk), ssize(chunk));

  File file;

  // Get file identifier and version
  file.id = {data(chunk), 2uz};
  file.version = static_cast<uint16_t>(chunk[2uz] << 16u | chunk[3uz] << 0u);

  // Flash
  auto const flash_start{
    static_cast<uint32_t>(static_cast<uint8_t>(chunk[5uz]) << 24u |
                          static_cast<uint8_t>(chunk[6uz]) << 16u |
                          static_cast<uint8_t>(chunk[7uz]) << 8u |
                          static_cast<uint8_t>(chunk[8uz]) << 0u)};
  auto const flash_length{
    static_cast<size_t>(static_cast<uint8_t>(chunk[9uz]) << 24u |
                        static_cast<uint8_t>(chunk[10uz]) << 16u |
                        static_cast<uint8_t>(chunk[11uz]) << 8u |
                        static_cast<uint8_t>(chunk[12uz]) << 0u)};
  file.flash.resize(static_cast<size_t>(flash_length));
  std::ranges::copy_n(
    cbegin(chunk) + flash_start,
    static_cast<std::iter_difference_t<decltype(chunk)>>(flash_length),
    begin(file.flash));

  // CVs
  auto const cvs_start{
    static_cast<uint32_t>(static_cast<uint8_t>(chunk[13uz]) << 24u |
                          static_cast<uint8_t>(chunk[14uz]) << 16u |
                          static_cast<uint8_t>(chunk[15uz]) << 8u |
                          static_cast<uint8_t>(chunk[16uz]) << 0u)};
  auto const cvs_length{
    static_cast<size_t>(static_cast<uint8_t>(chunk[17uz]) << 8u |
                        static_cast<uint8_t>(chunk[18uz]) << 0u)};
  for (auto i{0uz}; i < cvs_length; i += 3uz) {
    if (auto const cv_addr{static_cast<uint32_t>(
          static_cast<uint8_t>(chunk[cvs_start + i]) << 8u |
          static_cast<uint8_t>(chunk[cvs_start + i + 1uz]) << 0u)};
        cv_addr < cvs_length) {
      auto const byte{static_cast<uint8_t>(chunk[cvs_start + i + 2uz])};
      file.cvs.push_back({cv_addr, byte});
    }
  }

  // Coded
  file.coded = chunk[19uz];

  return file;
}

}  // namespace zpp