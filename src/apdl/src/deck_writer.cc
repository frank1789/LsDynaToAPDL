/**
 * @file deck_writer.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Implementation of the APDL deck writer.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "apdl/deck_writer.hh"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <fstream>
#include <ostream>
#include <string_view>
#include <vector>

#include "apdl/apdl_entity.hh"
#include "apdl/material.hh"

namespace lsdynatoapdl::apdl {
namespace {

/**
 * @brief Reference number of the SHELL181 element type.
 */
constexpr std::uint64_t kShellTypeId{1};

/**
 * @brief Reference number of the material assigned to every shell.
 */
constexpr std::uint64_t kDefaultMaterialId{1};

/**
 * @brief The rule separating the blocks of the deck.
 */
constexpr std::string_view kRule{
    "/COM,  "
    "-----------------------------------------------------------------------"
    "----------"};

/**
 * @brief Banner identifying the problem and how the deck was produced.
 */
void write_banner(std::ostream& os, const Model& model) {
  os << kRule << '\n'
     << "/COM,   PROBLEM: " << model.title() << '\n'
     << kRule << '\n';
  if (not model.source().empty()) {
    os << "/COM,   Converted from " << model.source() << " by LsDynaToAPDL\n";
  }
  os << "/COM,   nodes: " << model.nodes().size()
     << ", keypoints: " << model.keypoints().size()
     << ", shells: " << model.shells().size()
     << ", sections: " << model.sections().size() << '\n'
     << kRule << "\n\n";
}

/**
 * @brief Session setup: leave whatever was open, then start clean.
 */
void write_session(std::ostream& os, const Model& model) {
  os << "FINISH\n"
     << "/CLEAR, START, NEW\n"
     << "/FILNAM, " << model.jobname() << '\n'
     << "/TITLE, " << model.title() << '\n'
     << "/UNIT, " << model.units() << '\n'
     << "/INQUIRE, StrJobname, JOBNAME\n\n";
}

/**
 * @brief Scalar parameters the rest of the deck refers to.
 */
void write_parameters(std::ostream& os, const Model& model) {
  os << "! >>>>> MODEL PARAMETERS <<<<<\n"
     << "*SET, Pi, ACOS(-1)\t\t!Pi constant\n"
     << "*AFUN, DEG\t\t\t!Specify units for angular measures [DEG], specify "
        "after function *AFUN\n"
     << "*SET, eps, 10e-3\t\t!precision interval\n\n";

  os << "! >>>>> MATERIAL PROPERTIES <<<<<\n";
  if (model.materials().empty()) {
    // Saying so beats emitting a deck that silently has no material: the
    // sections below refer to material 1, and Ansys would stop there.
    os << "!*** none supplied; re-run with --material "
          "\"Name,YoungsModulus,PoissonRatio,Density\"\n\n";
  }
  for (const auto& material : model.materials()) {
    os << material << "\n\n";
  }
}

/**
 * @brief Everything that belongs inside the preprocessor.
 */
void write_model(std::ostream& os, const Model& model) {
  os << "/PREP7\n\n";

  if (not model.shells().empty()) {
    os << "! >>>>> element type definition <<<<<\n"
       << ElementType(kShellTypeId, kShell181Name) << "\n\n";
  }

  if (not model.materials().empty()) {
    os << "! >>>>> material assignment <<<<<\n";
    std::uint64_t material_id{1};
    for (const auto& material : model.materials()) {
      os << MaterialProperty(material_id, material.name()) << '\n';
      ++material_id;
    }
    os << '\n';
  }

  if (not model.shells().empty()) {
    os << "! >>>>> section definition <<<<<\n";
    for (const auto& section : model.sections()) {
      os << section << '\n';
    }
    os << '\n';
  }

  if (not model.keypoints().empty()) {
    os << "! >>>>> keypoint definition <<<<<\n";
    for (const auto& keypoint : model.keypoints()) {
      os << keypoint << '\n';
    }
    os << "! >>>>> end keypoint definition <<<<<\n\n";
  }

  if (not model.nodes().empty()) {
    os << "! >>>>> node definition <<<<<\n";
    for (const auto& node : model.nodes()) {
      os << node << '\n';
    }
    os << "! >>>>> end node definition <<<<<\n\n";
  }

  if (not model.shells().empty()) {
    os << "! >>>>> shell elements definition <<<<<\n"
       << "TYPE, " << kShellTypeId << '\n'
       << "MAT, " << kDefaultMaterialId << '\n';

    // TYPE/MAT/SECNUM are modal, so walk the elements in section order and
    // re-emit SECNUM only when the active section actually changes.
    auto ordered = model.shells();
    std::ranges::stable_sort(ordered, {}, [&model](const Shell181& shell) {
      return model.section_of(shell);
    });

    std::uint64_t active_section{0};
    for (const auto& shell : ordered) {
      if (const auto section = model.section_of(shell);
          section != active_section) {
        active_section = section;
        os << "SECNUM, " << active_section << '\n';
      }
      os << shell << '\n';
    }
    os << "! >>>>> end elements definition <<<<<\n\n";
  }

  os << "FINISH\n";
}

}  // namespace

void DeckWriter::set_filename(std::filesystem::path filename) {
  m_filename = std::move(filename);
  spdlog::debug("apdl output file set to \"{}\"", m_filename.string());
}

auto DeckWriter::write(const Model& model) const -> bool {
  if (m_filename.empty()) {
    spdlog::error("output filename is not set, cannot write the apdl deck");
    return false;
  }

  std::ofstream ofs(m_filename, std::ios::out | std::ios::trunc);
  if (not ofs) {
    spdlog::error("failed to open \"{}\" for writing", m_filename.string());
    return false;
  }

  write(model, ofs);
  ofs.flush();
  if (not ofs) {
    spdlog::error("failed while writing \"{}\"", m_filename.string());
    return false;
  }

  spdlog::info("written apdl deck \"{}\" ({} nodes, {} shells)",
               m_filename.string(), model.nodes().size(),
               model.shells().size());
  return true;
}

void DeckWriter::write(const Model& model, std::ostream& os) {
  write_banner(os, model);
  write_session(os, model);
  write_parameters(os, model);
  write_model(os, model);
}

}  // namespace lsdynatoapdl::apdl
