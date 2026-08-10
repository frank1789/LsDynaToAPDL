/**
 * @file main.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Command line entry point.
 * @version 0.4.1
 * @date 2022-07-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <fmt/format.h>
#include <gflags/gflags.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "apdl/material.hh"
#include "core/parser.hh"
#include "lsdyna/fields.hh"

DEFINE_string(input, "", "LS-DYNA keyword deck to convert (required)");
DEFINE_bool(verbose, false, "Log every parsed record");
DEFINE_string(log, "lsdynatoapdl.log", "Log file to write");
DEFINE_int32(thickness_precision, 6,
             "Decimal places kept when grouping shell thicknesses into APDL "
             "sections; lower values merge more elements");
DEFINE_string(material, "",
              "Material parameters, as Name,YoungsModulus,PoissonRatio,Density"
              " — repeat with ';' for several, e.g. "
              "\"Dural,72e9,0.33,2810;Aluminium,64e9,0.34,2700\"");
DEFINE_string(title, "", "Analysis title for /TITLE (default: from the input)");
DEFINE_string(jobname, "", "Job name for /FILNAM (default: from the input)");
DEFINE_string(units, "SI", "Unit system for /UNIT");

namespace lsdynatoapdl {
namespace {

constexpr std::string_view kTitle = R"(
   _       _____ _____                 _______                      _ _
  | |     / ____|  __ \               |__   __|     /\             | | |
  | |    | (___ | |  | |_   _ _ __   __ _| | ___   /  \   _ __   __| | |
  | |     \___ \| |  | | | | | '_ \ / _` | |/ _ \ / /\ \ | '_ \ / _` | |
  | |____ ____) | |__| | |_| | | | | (_| | | (_) / ____ \| |_) | (_| | |
  |______|_____/|_____/ \__, |_| |_|\__,_|_|\___/_/    \_\ .__/ \__,_|_|
                        __/ |                           | |
                       |___/                            |_|
 )";

constexpr std::string_view kAuthor =
    "author: Francesco Argentieri (francesco.argentieri89@gmail.com)";

/**
 * @brief Send log records to both the console and a file.
 *
 * @param log_file path of the log file.
 * @param verbose whether debug records also reach the console.
 */
void initialise_logger(const std::filesystem::path& log_file, bool verbose) {
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_level(verbose ? spdlog::level::debug : spdlog::level::info);
  console_sink->set_pattern("[%^%l%$] %v");

  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
      log_file.string(), true);
  file_sink->set_level(spdlog::level::trace);
  file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l] %v");

  const std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
  auto logger = std::make_shared<spdlog::logger>("lsdynatoapdl", sinks.begin(),
                                                 sinks.end());
  logger->set_level(verbose ? spdlog::level::trace : spdlog::level::info);
  logger->flush_on(spdlog::level::warn);
  spdlog::set_default_logger(logger);
}

/**
 * @brief Split @p text on @p separator, dropping empty pieces.
 */
[[nodiscard]] auto split(std::string_view text, char separator)
    -> std::vector<std::string_view> {
  std::vector<std::string_view> parts;
  for (std::size_t begin = 0; begin <= text.size();) {
    const auto end = text.find(separator, begin);
    const auto piece =
        text.substr(begin, end == std::string_view::npos ? end : end - begin);
    if (not piece.empty()) {
      parts.push_back(piece);
    }
    if (end == std::string_view::npos) {
      break;
    }
    begin = end + 1;
  }
  return parts;
}

/**
 * @brief Remove surrounding whitespace.
 */
[[nodiscard]] auto trim(std::string_view text) -> std::string_view {
  constexpr std::string_view kSpace{" \t"};
  const auto begin = text.find_first_not_of(kSpace);
  if (begin == std::string_view::npos) {
    return {};
  }
  return text.substr(begin, text.find_last_not_of(kSpace) - begin + 1);
}

/**
 * @brief Parse the --material flag into materials.
 *
 * @details The syntax is `Name,E,nu,rho`, repeated with ';'. Values are
 * validated as numbers but stored as written, so `72e9` reaches the deck as
 * `72e9` rather than as 72000000000.
 *
 * @param spec the flag's value.
 * @param materials receives the parsed materials.
 * @return true when every entry was well formed.
 */
[[nodiscard]] auto parse_materials(std::string_view spec,
                                   std::vector<apdl::Material>& materials)
    -> bool {
  constexpr std::size_t kFieldsPerMaterial{4};
  auto ok = true;

  for (const auto& entry : split(spec, ';')) {
    const auto fields = split(entry, ',');
    if (fields.size() != kFieldsPerMaterial) {
      spdlog::error(
          R"(--material "{}": expected Name,YoungsModulus,PoissonRatio,Density )"
          "but found {} field(s)",
          entry, fields.size());
      ok = false;
      continue;
    }

    const auto name = trim(fields[0]);
    if (name.empty()) {
      spdlog::error(R"(--material "{}": the name is empty)", entry);
      ok = false;
      continue;
    }

    auto numeric = true;
    for (std::size_t index = 1; index < kFieldsPerMaterial; ++index) {
      if (not lsdyna::to_double(trim(fields[index])).has_value()) {
        spdlog::error(R"(--material "{}": "{}" is not a number)", entry,
                      trim(fields[index]));
        numeric = false;
      }
    }
    if (not numeric) {
      ok = false;
      continue;
    }

    materials.emplace_back(std::string{name}, std::string{trim(fields[1])},
                           std::string{trim(fields[2])},
                           std::string{trim(fields[3])});
  }

  return ok;
}

}  // namespace
}  // namespace lsdynatoapdl

auto main(int argc, char* argv[]) -> int {
  gflags::SetUsageMessage(
      "Convert a LS-DYNA keyword deck into an Ansys APDL deck.\n"
      "  lsdynatoapdl --input model.k");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  fmt::print("{}\n{}\n", lsdynatoapdl::kTitle, lsdynatoapdl::kAuthor);

  auto exit_code = EXIT_SUCCESS;
  try {
    lsdynatoapdl::initialise_logger(FLAGS_log, FLAGS_verbose);

    if (FLAGS_input.empty()) {
      spdlog::error("no input deck given; use --input <file.k> or --help");
      exit_code = EXIT_FAILURE;
    } else {
      std::vector<lsdynatoapdl::apdl::Material> materials;
      if (not lsdynatoapdl::parse_materials(FLAGS_material, materials)) {
        return EXIT_FAILURE;
      }
      if (materials.empty()) {
        spdlog::warn(
            "no --material given; the deck will declare none and Ansys will "
            "stop at the first section that refers to material 1");
      }

      auto parser = std::make_unique<lsdynatoapdl::Parser>();
      parser->set_thickness_decimals(FLAGS_thickness_precision);
      parser->set_materials(std::move(materials));
      parser->set_title(FLAGS_title);
      parser->set_jobname(FLAGS_jobname);
      parser->set_units(FLAGS_units);
      exit_code = parser->elaborate(FLAGS_input) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
  } catch (const std::exception& ex) {
    spdlog::error("{}", ex.what());
    exit_code = EXIT_FAILURE;
  }

  spdlog::shutdown();
  gflags::ShutDownCommandLineFlags();
  return exit_code;
}
