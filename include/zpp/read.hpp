// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Read ZPP file
///
/// \file   zpp/zpp.hpp
/// \author Vincent Hamp
/// \date   17/11/1024

#pragma once

#include <filesystem>
#include "file.hpp"

namespace zpp {

/// Read ZPP file
///
/// Throws std::filesystem::filesystem_error on invalid paths.
///
/// \param  path  Path to ZPP file
/// \return ZPP file
File read(std::filesystem::path path);

}  // namespace zpp