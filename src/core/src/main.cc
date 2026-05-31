/**
 * @file main.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The main file.
 * @version 0.4.1
 * @date 2022-07-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <iostream>
#include <lsdyna/lsdyna.hh>
#include <memory>
#include <string_view>

#include "apdl/writeapdl.h"
#include "core/parser.h"

// Add missing namespaces and setup_logger stubs
namespace lsdynatoapdl {

constexpr std::string_view Title = R"(
   _       _____ _____                 _______                      _ _
  | |     / ____|  __ \               |__   __|     /\             | | |
  | |    | (___ | |  | |_   _ _ __   __ _| | ___   /  \   _ __   __| | |
  | |     \___ \| |  | | | | | '_ \ / _` | |/ _ \ / /\ \ | '_ \ / _` | |
  | |____ ____) | |__| | |_| | | | | (_| | | (_) / ____ \| |_) | (_| | |
  |______|_____/|_____/ \__, |_| |_|\__,_|_|\___/_/    \_\ .__/ \__,_|_|
                        __/ |                           | |
                       |___/                            |_|
 )";

constexpr std::string_view Author =
    "Francesco Argentieri (francesco.argentieri89@gmail.com)";

}  // namespace lsdynatoapdl

auto main(int argc, char* argv[]) -> int {
  try {
    fmt::println("{}", lsdynatoapdl::Title);
    fmt::println("{}", lsdynatoapdl::Author);

    const auto FileLog =
        (std::filesystem::current_path() / "lsdynatoapdl.log").string();
    fmt::println("Log file: {}", FileLog);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

    auto file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(FileLog, true);
    file_sink->set_level(spdlog::level::trace);

    spdlog::sinks_init_list sink_list = {file_sink, console_sink};

    spdlog::logger logger("multi_sink", sink_list.begin(), sink_list.end());
    logger.set_level(spdlog::level::trace);
    // logger.warn("this should appear in both console and file");
    // logger.info("this message should not appear in the console, only in the
    // file"); logger.debug("debug message");

    // or you can even set multi_sink logger as default logger
    spdlog::set_default_logger(std::make_shared<spdlog::logger>(
        "multi_sink", spdlog::sinks_init_list({console_sink, file_sink})));

    auto dyna_logger = lsdynatoapdl::lsdyna::setup_logger(sink_list);
    // auto logger = lsdynatoapdl::apdl::setup_logger(sinks);
    auto p = spdlog::get("lsdyna");
    p->info("Hello from lsdyna logger");
    p->debug("zazz");
    p->trace("trace message");
    p->warn("warn message");
    p->error("error message");
    // logger = lsdynatoapdl::lsdyna::setup_logger(sinks);
    // spdlog::flush_on(spdlog::level::info);

    // spdlog::register_logger(apdl_logger);
    //

    // spdlog::register_logger(dyna_logger);
    // spdlog::set_default_logger(logger);

    auto parser = std::make_unique<lsdynatoapdl::Parser>();
    // auto writer = std::make_unique<lsdynatoapdl::Writer>();
    parser->elaborate("");

  } catch (const std::exception& ex) {
    spdlog::error("{}", ex.what());
    spdlog::dump_backtrace();

    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
