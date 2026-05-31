/**
 * @file lsdyna.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-08-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_LSDYNA_HH
#define LSDYNA_TO_APDL_LSDYNA_HH

#include <memory>
#include <vector>

#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace lsdynatoapdl {
namespace lsdyna {

static constexpr auto* logger_name{"lsdyna"};

std::shared_ptr<spdlog::logger> setup_logger(
    std::vector<spdlog::sink_ptr> sinks);

}  // namespace lsdyna
}  // namespace lsdynatoapdl

#endif  // LSDYNA_TO_APDL_LSDYNA_HH
