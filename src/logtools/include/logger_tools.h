/**
 * @file logger_tools.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The extend logger tools.
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LOGGER_TOOLS_H
#define LOGGER_TOOLS_H

// #if !defined(SPDLOG_ACTIVE_LEVEL)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
// #endif
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define CLIENT_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define CLIENT_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define CLIENT_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define CLIENT_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define CLIENT_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define CLIENT_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define CLIENT_FUNC_SIG __func__
#else
#define CLIENT_FUNC_SIG "FUNC_SIG unknown!"
#endif

struct LoggerManager {
  void initialize() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // console_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] %v%$");
    console_sink->set_level(spdlog::level::trace);

    const std::size_t max_size = 10 * 1024 * 1024;
    const std::size_t max_files = 10;
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("/Users/francesco/Documents/Projects/LsDynaToAPDL/tmplog.txt", max_size, max_files);
    //    file_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%@] %v%$");
    file_sink->set_level(spdlog::level::trace);
    spdlog::sinks_init_list sink_list = {file_sink, console_sink};
    spdlog::logger logger("multi_sink", sink_list.begin(), sink_list.end());
    logger.set_level(spdlog::level::trace);

    // or you can even set multi_sink logger as default logger
    spdlog::set_default_logger(std::make_shared<spdlog::logger>(
        "general", spdlog::sinks_init_list({console_sink, file_sink})));

    // logger.flush_on(spdlog::level::off);
    logger.enable_backtrace(32);

    logger.dump_backtrace();
    spdlog::info("hello");
  }

  void shutdown() { spdlog::shutdown(); }
};

#endif  // LOGGER_TOOLS_H
