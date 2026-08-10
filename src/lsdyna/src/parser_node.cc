/**
 * @file parser_node.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Implementation of the *NODE parser.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "lsdyna/parser_node.hh"

#include <spdlog/spdlog.h>

#include "apdl/node.hh"
#include "lsdyna/fields.hh"

namespace lsdynatoapdl::lsdyna {
namespace {

/// nid, x, y, z.
constexpr std::size_t kMinimumFields{4};

}  // namespace

void ParserNode::parse(std::string_view line) {
  if (is_skippable(line)) {
    return;
  }

  const auto fields = split_fields(line);
  if (fields.size() < kMinimumFields) {
    ++m_rejected;
    spdlog::debug("node card has {} fields, expected at least {}: \"{}\"",
                  fields.size(), kMinimumFields, line);
    return;
  }

  const auto id = to_uint(fields[0]);
  const auto x = to_double(fields[1]);
  const auto y = to_double(fields[2]);
  const auto z = to_double(fields[3]);

  if (not id or not x or not y or not z) {
    ++m_rejected;
    spdlog::debug("node card is not numeric: \"{}\"", line);
    return;
  }

  m_model->add_node(apdl::Node{*id, *x, *y, *z});
  ++m_accepted;
}

}  // namespace lsdynatoapdl::lsdyna
