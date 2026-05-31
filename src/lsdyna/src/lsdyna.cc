#include "lsdyna/lsdyna.hh"

namespace lsdynatoapdl {
namespace lsdyna {

std::shared_ptr<spdlog::logger> setup_logger(
    std::vector<spdlog::sink_ptr> sinks) {
  auto logger = spdlog::get(logger_name);
  if (not logger) {
    if (sinks.size() > 0) {
      logger = std::make_shared<spdlog::logger>(logger_name, std::begin(sinks),
                                                std::end(sinks));
      spdlog::register_logger(logger);
    } else {
      logger = spdlog::stdout_color_mt(logger_name);
    }
  }
  logger->set_level(spdlog::level::trace);
  return logger;
}

}  // namespace lsdyna
}  // namespace lsdynatoapdl
