#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H

#include <memory>
#include <unordered_map>

#include "finite_element_types.h"

class alignas(std::max_align_t) ElementFactory {
 public:
  explicit ElementFactory() noexcept;
  ~ElementFactory() = default;

  std::unique_ptr<Element> createElement(ShellType elem_type);

 private:
  std::unordered_map<ShellType, std::unique_ptr<Element>, std::hash<ShellType>> prototypes_;
};

#endif  // ELEMENT_FACTORY_H
