#include "lsdyna/lsdyna.h"

namespace lsdynatoapdl {
namespace lsdyna {
std::shared_ptr<spdlog::logger> setup_logger(
    std::vector<spdlog::sink_ptr> sinks) {
  auto logger = spdlog::get(logger_name.data());
  if (not logger) {
    if (sinks.size() > 0) {
      logger = std::make_shared<spdlog::logger>(logger_name.data(), std::begin(sinks),
                                                std::end(sinks));
      spdlog::register_logger(logger);
    } else {
      logger = spdlog::stdout_color_mt(logger_name.data());
    }
  }

  return logger;
}
}  // namespace lsdyna
}  // namespace lsdynatoapdl
