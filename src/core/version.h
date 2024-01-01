/**
 * @file version.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Application version
 * @version 0.1.0
 * @date 2021-11-13
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef LSDYNA_TO_APDL_CONVERTER_VERSION_H
#define LSDYNA_TO_APDL_CONVERTER_VERSION_H

#include <fmt/core.h>

#include <string_view>

inline constexpr char kVersionMajor[] = "0";
inline constexpr char kVersionMinor[] = "1";
inline constexpr char kVersionPatch[] = "0";
inline constexpr char kBuild[] = "2084bd3";

inline constexpr auto compact_version() {
  return fmt::format("{}.{}.{} build: {}", kVersionMajor, kVersionMinor, kVersionPatch, kBuild);
}

#endif  // LSDYNA_TO_APDL_CONVERTER_VERSION_H
