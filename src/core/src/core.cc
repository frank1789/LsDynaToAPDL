#include "core/core.h"

#include "apdl/writeapdl.h"
#include "core/parser.h"
#include "lsdyna/lsdyna.hh"

#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <iostream>
#include <memory>

DEFINE_bool(verbose, false, "Display program name before message");
DEFINE_string(infile, "Hello world!", "Message to print");

static bool IsNonEmptyMessage(const char* flagname, const std::string& value) {
  return value[0] != '\0';
}
DEFINE_validator(infile, &IsNonEmptyMessage);

namespace lsdynatoapdl {

// static void InitializeLogger() {
//   const auto kFileLog =
//       (std::filesystem::current_path() / "lsdynatoapdl.log").string();

//   // init console sink
//   auto console_sink =
//   std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//   console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%P][%^%l%$] %v");
//   console_sink->set_level(spdlog::level::info);

//   // init file sink
//   auto file_sink =
//       std::make_shared<spdlog::sinks::basic_file_sink_mt>(kFileLog);
//   file_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e][%n][%P][%l] %v");
//   file_sink->set_level(spdlog::level::debug);

//   std::vector<spdlog::sink_ptr> sinks{file_sink, console_sink};
//   // register loggers
//   // auto apdl_logger = apdl::setup_logger(sinks);
//   auto logger = lsdyna::setup_logger(sinks);
//   spdlog::flush_on(spdlog::level::trace);

//   // spdlog::register_logger(apdl_logger);
//   //spdlog::register_logger(dyna_logger);
//   spdlog::set_default_logger(logger);
// }

// auto main_app(int argc, char** argv) -> int {
//   try {
//     InitializeLogger();
//     // LoggerManager::initialize();
//     std::cout <<
//         R"(
//    _       _____ _____                 _______                      _ _
//   | |     / ____|  __ \               |__   __|     /\             | | |
//   | |    | (___ | |  | |_   _ _ __   __ _| | ___   /  \   _ __   __| | |
//   | |     \___ \| |  | | | | | '_ \ / _` | |/ _ \ / /\ \ | '_ \ / _` | |
//   | |____ ____) | |__| | |_| | | | | (_| | | (_) / ____ \| |_) | (_| | |
//   |______|_____/|_____/ \__, |_| |_|\__,_|_|\___/_/    \_\ .__/ \__,_|_|
//                         __/ |                           | |
//                        |___/                            |_|
//  )" << std::endl;
//     std::cout
//         << "author: Francesco Argentieri
//         (francesco.argentieri89@gmail.com)\n";

//     auto parser = std::make_unique<lsdynatoapdl::Parser>();

//     parser->elaborate(FLAGS_infile);
//     // LoggerManager::shutdown();
//     gflags::SetUsageMessage("some usage message");
//     gflags::SetVersionString("1.0.0");
//     gflags::ParseCommandLineFlags(&argc, &argv, true);
//     if (FLAGS_verbose)
//       std::cout << gflags::ProgramInvocationShortName() << ": ";
//     std::cout << FLAGS_infile << std::endl;
//     gflags::ShutDownCommandLineFlags();
//   } catch (const std::exception& ex) {
//     spdlog::dump_backtrace();
//     spdlog::error("{}", ex.what());
//     //  LoggerManager::shutdown();
//     return EXIT_FAILURE;
//   }
//   return EXIT_SUCCESS;
// }

}  // namespace lsdynatoapdl
