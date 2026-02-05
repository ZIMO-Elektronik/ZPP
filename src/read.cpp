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

  // Developer code
  file.developer_code = {static_cast<uint8_t>(chunk[flash_start + 2190uz]),
                         static_cast<uint8_t>(chunk[flash_start + 2191uz]),
                         static_cast<uint8_t>(chunk[flash_start + 2192uz]),
                         static_cast<uint8_t>(chunk[flash_start + 2193uz])};

  // WAV file names
  file.wav_file_names.resize(259uz);
  auto wav_addr{static_cast<uint32_t>(static_cast<uint8_t>(chunk[28uz]) << 24u |
                                      static_cast<uint8_t>(chunk[29uz]) << 16u |
                                      static_cast<uint8_t>(chunk[30uz]) << 8u |
                                      static_cast<uint8_t>(chunk[31uz]) << 0u)};
  auto const wav_count{
    static_cast<size_t>(chunk[32uz] << 8u | chunk[33uz] << 0u)};
  for (auto i{0uz}; i < wav_count; ++i) {
    auto const number{
      static_cast<uint16_t>(static_cast<uint8_t>(chunk[wav_addr + 0uz]) << 8u |
                            static_cast<uint8_t>(chunk[wav_addr + 1uz]) << 0u)};
    if (!number || number > 256u)
      throw std::logic_error("Invalid audio file number");
    wav_addr += sizeof(number);
    auto const placeholder{
      static_cast<uint32_t>(static_cast<uint8_t>(chunk[wav_addr + 0uz]) << 24u |
                            static_cast<uint8_t>(chunk[wav_addr + 1uz]) << 16u |
                            static_cast<uint8_t>(chunk[wav_addr + 2uz]) << 8u |
                            static_cast<uint8_t>(chunk[wav_addr + 3uz]) << 0u)};
    wav_addr += sizeof(placeholder);
    auto const count{static_cast<uint8_t>(chunk[wav_addr])};
    wav_addr += sizeof(count);
    file.wav_file_names[number - 1uz] = std::string(&chunk[wav_addr], count);
    wav_addr += count;
  }

  // Author
  auto const author_addr{
    static_cast<uint32_t>(static_cast<uint8_t>(chunk[34uz]) << 24u |
                          static_cast<uint8_t>(chunk[35uz]) << 16u |
                          static_cast<uint8_t>(chunk[36uz]) << 8u |
                          static_cast<uint8_t>(chunk[37uz]) << 0u)};
  auto const author_count{static_cast<uint8_t>(chunk[author_addr])};
  file.author = std::string(&chunk[author_addr + 1uz], author_count);

  // E-mail
  auto const mail_addr{
    static_cast<uint32_t>(static_cast<uint8_t>(chunk[38uz]) << 24u |
                          static_cast<uint8_t>(chunk[39uz]) << 16u |
                          static_cast<uint8_t>(chunk[40uz]) << 8u |
                          static_cast<uint8_t>(chunk[41uz]) << 0u)};
  auto const mail_count{static_cast<uint8_t>(chunk[author_addr])};
  file.email = std::string(&chunk[mail_addr + 1uz], mail_count);

  return file;
}

} // namespace zpp
