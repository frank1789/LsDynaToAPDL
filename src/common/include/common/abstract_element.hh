/**
 * @file abstract_element.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_COMMON_ABSTRACT_ELEMENT_HH
#define LSDYNA_TO_APDL_COMMON_ABSTRACT_ELEMENT_HH

#include <cstdint>

class AbstractElement {
 public:
  virtual ~AbstractElement() noexcept = default;

  AbstractElement(const AbstractElement&) = delete;
  AbstractElement(AbstractElement&&) noexcept = delete;

  AbstractElement& operator=(const AbstractElement&) = delete;
  AbstractElement& operator=(AbstractElement&&) noexcept = delete;

  virtual void parse_element(std::string_view input_line) = 0;

 protected:
  constexpr AbstractElement() noexcept = default;
};

#endif  // LSDYNA_TO_APDL_COMMON_ABSTRACT_ELEMENT_HH
