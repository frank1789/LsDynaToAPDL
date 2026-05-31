/**
 * @file element_shell_four_node.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_COMMON_ELEMENT_SHELL_FOUR_NODE_HH
#define LSDYNA_TO_APDL_COMMON_ELEMENT_SHELL_FOUR_NODE_HH

#include <ostream>

#include "common/element_shell.hh"

class ShellFourNode : public Shell {
 public:
  /**
   * @brief
   *
   * @param input_line
   */
  void parse_element(std::string_view input_line) override;

  [[nodiscard]] constexpr std::uint64_t get_id() const override { return m_id; }

  [[nodiscard]] constexpr double get_thickness() const override {
    return m_thickness;
  }

 private:
  void extract_node(const std::string& input_line);
  void extract_thickness(const std::string& input_line);

  std::uint64_t m_id{}; /**< ID of the element. */
  double m_thickness{}; /**< thickness of the element. */

  bool m_node_flag{false};      /**< confirm extraction of node's connection. */
  bool m_thickness_flag{false}; /**< confirm extraction shell's thickness. */
};

// QDebug &operator<<(QDebug &os, const ShellFourNode &s);

using ElementShell181 = ShellFourNode;

#endif  // LSDYNA_TO_APDL_COMMON_ELEMENT_SHELL_FOUR_NODE_HH
