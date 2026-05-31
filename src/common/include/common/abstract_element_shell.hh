/**
 * @file abstract_element_shell.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_COMMON_ABSTRACT_ELEMENT_SHELL_HH
#define LSDYNA_TO_APDL_COMMON_ABSTRACT_ELEMENT_SHELL_HH

#include "common/abstract_element.hh"

class ElementShell : public AbstractElement {
 public:
  virtual ~ElementShell() noexcept = default;

  ElementShell(const ElementShell&) = delete;
  ElementShell(ElementShell&&) noexcept = delete;

  ElementShell& operator=(const ElementShell&) = delete;
  ElementShell& operator=(ElementShell&&) noexcept = delete;

  [[nodiscard]] virtual constexpr std::uint64_t get_id() const = 0;
  [[nodiscard]] virtual constexpr double get_thickness() const = 0;

 protected:
  constexpr ElementShell() noexcept = default;
};

#endif  // LSDYNA_TO_APDL_COMMON_ABSTRACT_ELEMENT_SHELL_HH
