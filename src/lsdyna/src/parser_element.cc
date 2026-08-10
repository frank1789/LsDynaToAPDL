/**
 * @file parser_element.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Implementation of the *ELEMENT_SHELL_THICKNESS parser.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "lsdyna/parser_element.hh"

#include <spdlog/spdlog.h>

#include "lsdyna/fields.hh"

namespace lsdynatoapdl::lsdyna {
namespace {

/// eid, pid and the four corner nodes.
constexpr std::size_t kConnectivityFields{6};

/// One thickness per corner node.
constexpr std::size_t kThicknessFields{4};

/// Index of the first node in the connectivity card.
constexpr std::size_t kFirstNodeField{2};

}  // namespace

void ParserElement::parse(std::string_view line) {
  if (is_skippable(line)) {
    return;
  }

  if (m_awaiting_thickness) {
    read_thickness(line);
  } else {
    read_connectivity(line);
  }
}

void ParserElement::read_connectivity(std::string_view line) {
  const auto fields = split_fields(line);
  if (fields.size() < kConnectivityFields) {
    ++m_rejected;
    spdlog::debug("shell connectivity has {} fields, expected {}: \"{}\"",
                  fields.size(), kConnectivityFields, line);
    return;
  }

  const auto id = to_uint(fields[0]);
  if (not id) {
    ++m_rejected;
    spdlog::debug("shell element id is not numeric: \"{}\"", line);
    return;
  }

  apdl::Shell181::NodeIds nodes{};
  for (std::size_t index = 0; index < nodes.size(); ++index) {
    const auto node = to_uint(fields[kFirstNodeField + index]);
    if (not node) {
      ++m_rejected;
      spdlog::debug("shell node {} is not numeric: \"{}\"", index + 1, line);
      return;
    }
    nodes[index] = *node;
  }

  m_pending_id = *id;
  m_pending_nodes = nodes;
  m_awaiting_thickness = true;
}

void ParserElement::read_thickness(std::string_view line) {
  // Whatever happens to this line, the pending element is resolved: leaving
  // the flag set would make the next connectivity card be read as thicknesses
  // and desynchronise the rest of the section.
  m_awaiting_thickness = false;

  const auto fields = split_fields(line);
  if (fields.size() < kThicknessFields) {
    ++m_rejected;
    spdlog::debug("shell thickness card has {} fields, expected {}: \"{}\"",
                  fields.size(), kThicknessFields, line);
    return;
  }

  auto total = 0.0;
  for (std::size_t index = 0; index < kThicknessFields; ++index) {
    const auto thickness = to_double(fields[index]);
    if (not thickness) {
      ++m_rejected;
      spdlog::debug("shell thickness {} is not numeric: \"{}\"", index + 1,
                    line);
      return;
    }
    total += *thickness;
  }

  m_model->add_shell(m_pending_id, m_pending_nodes,
                     total / static_cast<double>(kThicknessFields));
  ++m_accepted;
}

}  // namespace lsdynatoapdl::lsdyna
