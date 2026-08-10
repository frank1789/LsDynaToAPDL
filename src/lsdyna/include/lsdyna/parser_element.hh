/**
 * @file parser_element.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Parser for the *ELEMENT_SHELL_THICKNESS section of a LS-DYNA deck.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_LSDYNA_PARSER_ELEMENT_HH
#define LSDYNA_TO_APDL_LSDYNA_PARSER_ELEMENT_HH

#include <cstddef>
#include <cstdint>
#include <string_view>

#include "apdl/model.hh"
#include "apdl/shell181.hh"
#include "lsdyna/parser.hh"

namespace lsdynatoapdl::lsdyna {

/**
 * @brief Reads `*ELEMENT_SHELL_THICKNESS` cards into an APDL model.
 *
 * @details Unlike `*NODE`, one element spans **two** consecutive lines:
 *
 * @code
 *  1229174       4 1189339 1162886 1189341 1189340
 *  ^eid          ^pid ^n1     ^n2     ^n3     ^n4
 *        3.9788582       3.9788582       3.9788582       3.9788582
 *        ^t1              ^t2             ^t3             ^t4
 * @endcode
 *
 * The parser is therefore a two-state machine: the first line establishes the
 * connectivity, the second supplies a thickness per corner node. The element
 * is only committed once both halves are present, so a truncated deck never
 * produces a shell with a default thickness of zero.
 *
 * SHELL181 sections carry a single thickness, so the four nodal values are
 * averaged; for the constant-thickness shells this converter targets they are
 * equal anyway, and averaging degrades gracefully when they are not.
 */
class ParserElement final : public Parser {
 public:
  /**
   * @brief Construct a parser filling @p model.
   *
   * @param model destination model; must outlive the parser.
   */
  explicit ParserElement(apdl::Model& model) noexcept : m_model(&model) {}

  void parse(std::string_view line) override;

  [[nodiscard]] auto accepted() const noexcept -> std::size_t override {
    return m_accepted;
  }

  /**
   * @brief Number of lines that looked like data but could not be read.
   */
  [[nodiscard]] auto rejected() const noexcept -> std::size_t {
    return m_rejected;
  }

  /**
   * @brief Whether a connectivity line is waiting for its thickness line.
   */
  [[nodiscard]] auto awaiting_thickness() const noexcept -> bool {
    return m_awaiting_thickness;
  }

 private:
  void read_connectivity(std::string_view line);
  void read_thickness(std::string_view line);

  apdl::Model* m_model;
  std::uint64_t m_pending_id{};
  apdl::Shell181::NodeIds m_pending_nodes{};
  bool m_awaiting_thickness{false};
  std::size_t m_accepted{};
  std::size_t m_rejected{};
};

}  // namespace lsdynatoapdl::lsdyna

#endif  // LSDYNA_TO_APDL_LSDYNA_PARSER_ELEMENT_HH
