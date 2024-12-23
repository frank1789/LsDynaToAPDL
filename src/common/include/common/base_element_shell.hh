/**
 * @file base_element_shell.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_COMMON_BASE_ELEMENT_SHELL_HH
#define LSDYNA_TO_APDL_COMMON_BASE_ELEMENT_SHELL_HH

#include <string_view>

template <typename Derived>
class Element: public AbstractElement {
 public:
  void parse_element(std::string_view &input_line) {
    const auto impl = static_cast<const Derived &>(*this);
    impl.parse_element(input_line);
  }

  void set_id(std::uint64_t id) {
    const auto impl = static_cast<const Derived &>(*this);
    impl.SetId(id);
  }

  void set_nodes(std::initializer_list<uint64_t> const &li) {
    const auto impl = static_cast<const Derived &>(*this);
    impl.SetNodes(li);
  }

  void set_thickness(double thickness) {
    const auto impl = static_cast<const Derived &>(*this);
    impl.Set(thickness);
  }

  [[nodiscard]] constexpr auto get_id() const -> std::uint64_t {
    auto impl = static_cast<const Derived &>(*this);
    return impl.get_id();
  }

  [[nodiscard]] constexpr auto get_thickness() const -> double {
    auto impl = static_cast<const Derived &>(*this);
    return impl.get_thickness();
  }
  // [[nodiscard]] virtual auto getNodesId() const = 0;
};

#endif  // LSDYNA_TO_APDL_COMMON_BASE_ELEMENT_SHELL_HH
