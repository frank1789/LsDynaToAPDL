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

#include <cstdlib>
#include <iostream>
#include <memory>

// #include "logger_tools.h"
#include "parser.h"
#include "version.h"

#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
  try {
    // auto logger = std::make_unique<LoggerManager>();
    // logger->initialize();
    spdlog::enable_backtrace(32);
    std::cout <<
        R"( 
   _       _____ _____                 _______                      _ _  
  | |     / ____|  __ \               |__   __|     /\             | | | 
  | |    | (___ | |  | |_   _ _ __   __ _| | ___   /  \   _ __   __| | | 
  | |     \___ \| |  | | | | | '_ \ / _` | |/ _ \ / /\ \ | '_ \ / _` | | 
  | |____ ____) | |__| | |_| | | | | (_| | | (_) / ____ \| |_) | (_| | | 
  |______|_____/|_____/ \__, |_| |_|\__,_|_|\___/_/    \_\ .__/ \__,_|_| 
                        __/ |                           | |              
                       |___/                            |_|              
 )" << std::endl;
    std::cout << "version " << compact_version() << "\n";
    std::cout << "author: "
              << "Francesco Argentieri (francesco.argentieri89@gmail.com)\n";

    spdlog::info("hello world!");

    auto parser = std::make_unique<lsdynatoapdl::Parser>();
    // auto writer = std::make_unique<lsdynatoapdl::Writer>();
    parser->elaborate();

    // logger->shutdown();
  } catch (const std::exception& ex) {
    spdlog::dump_backtrace();
    spdlog::error("{}", ex.what());
  }
  return EXIT_SUCCESS;
}
