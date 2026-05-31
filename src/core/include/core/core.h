/**
 * @file core.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-08-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_CORE_H_
#define LSDYNA_TO_APDL_CORE_H_

#include <string>

namespace lsdynatoapdl {

constexpr std::string_view LoggerName{"core"};

auto main_app(int argc, char** argv) -> int;

auto register_logger(const std::string& name);

}  // namespace lsdynatoapdl

#endif  // LSDYNA_TO_APDL_CORE_H_
