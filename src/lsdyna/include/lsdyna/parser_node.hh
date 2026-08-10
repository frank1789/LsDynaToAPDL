/**
 * @file parser_node.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Parser for the *NODE section of a LS-DYNA deck.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_LSDYNA_PARSER_NODE_HH
#define LSDYNA_TO_APDL_LSDYNA_PARSER_NODE_HH

#include <cstddef>
#include <string_view>

#include "apdl/model.hh"
#include "lsdyna/parser.hh"

namespace lsdynatoapdl::lsdyna {

/**
 * @brief Reads `*NODE` cards and appends the nodes to an APDL model.
 *
 * @details A node card carries eight fields:
 *
 * @code
 *  1162886     -25.5204048      320.880554     -161.429962     0.0     0.0
 *  ^nid         ^x               ^y             ^z             ^tc     ^rc
 * @endcode
 *
 * `tc` and `rc` are the LS-DYNA translational and rotational constraint codes.
 * They describe boundary conditions, not geometry, and have no counterpart in
 * the APDL `N` command, so they are read past and dropped. Everything after
 * the fourth field is ignored, which keeps the parser tolerant of the extra
 * columns some pre-processors append.
 */
class ParserNode final : public Parser {
 public:
  /**
   * @brief Construct a parser filling @p model.
   *
   * @param model destination model; must outlive the parser.
   */
  explicit ParserNode(apdl::Model& model) noexcept : m_model(&model) {}

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

 private:
  apdl::Model* m_model;
  std::size_t m_accepted{};
  std::size_t m_rejected{};
};

}  // namespace lsdynatoapdl::lsdyna

#endif  // LSDYNA_TO_APDL_LSDYNA_PARSER_NODE_HH
