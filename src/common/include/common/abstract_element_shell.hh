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

#include <cstdint>

class AbstractElementShell {
 public:
  virtual ~AbstractElementShell() noexcept = default;

  AbstractElementShell(const AbstractElementShell&) = delete;
  AbstractElementShell(AbstractElementShell&&) noexcept = delete;

  AbstractElementShell& operator=(const AbstractElementShell&) = delete;
  AbstractElementShell& operator=(AbstractElementShell&&) noexcept = delete;

  [[nodiscard]] virtual constexpr std::uint64_t get_id() const = 0;
  [[nodiscard]] virtual constexpr double get_thickness() const = 0;

 protected:
  constexpr AbstractElementShell() noexcept = default;

  //   virtual std::unique_ptr<Element> clone() = 0;

  //   virtual void setId(uint64_t id) = 0;
  //   virtual void setNodes(std::initializer_list<uint64_t> const &li) = 0;
  //   virtual void setThickness(double thickness) = 0;

  // [[nodiscard]] virtual auto getNodesId() const = 0;

  //   virtual void parseElement(const QString &inputline) = 0;
};

#endif  // LSDYNA_TO_APDL_COMMON_ABSTRACT_ELEMENT_SHELL_HH
