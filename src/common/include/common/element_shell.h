#ifndef LSDYNA_TO_APDL_SHELL_ELEMENT_H_
#define LSDYNA_TO_APDL_SHELL_ELEMENT_H_

#include <cstdint>
#include <vector>

#include "elementproperty.h"

/**
 * @brief The propelem struct defines the general appearance of the parameters.
 * of an element.
 *
 * @details The content of the structure represents the element defined by a
 * unique ID, by the number of nodes that constitute it. The thickness used for
 * shell-like elements.
 * A template formulation is used to make most of the definitions available to
 * the structure.
 * It is suggested to use:
 *  - Integer data types:
 *      - element IDs;
 *      - nodes that constitute it as identified by the ID;
 *      - number of nodes of the element;
 *  - Float Data type:
 *      - thickness.
 *
 * @tparam N: type for elements IDs
 * @tparam EN: type for node IDs
 * @tparam T: type for element's thickness
 * @tparam size: sets the number of nodes of the element
 */
// template <typename N, typename EN, typename T, int size>
// class ShellElement : public Element<typename N, typename EN> {
//  private:
//   T thickness_; /**< thickness of the element. */
// };

namespace lsdynatoapdl {

template <typename Derived>
class Element {
 public:
  void SetId(std::uint64_t id) {
    auto impl = static_cast<Derived>(this);
    impl.SetId(id);
  }

  void SetNodes(std::initializer_list<uint64_t> const &li) {
    auto impl = static_cast<Derived>(this);
    impl.SetNodes(li);
  }

  void SetThickness(double thickness) {
    auto impl = static_cast<Derived>(this);
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

template <std::uint32_t NumberNode>
class Shell : public Element<Shell<NumberNode>>, public AbstractElement {
  friend class Element<Shell>;

 public:
 explicit Shell() = default;
  Shell(std::uint64_t id, std::vector<std::uint64_t> nodes, double thickness) :AbstractElement(id, nodes, thickness) {};
  ~Shell() noexcept  = default;

  //   virtual void setId(uint64_t id) = 0;
  //   virtual void setNodes(std::initializer_list<uint64_t> const &li) = 0;
  //   virtual void setThickness(double thickness) = 0;

  [[nodiscard]] constexpr auto get_id() const -> std::uint64_t { return m_id; }

  [[nodiscard]] constexpr auto get_thickness() const -> double {
    return m_thickness;
  }
  // [[nodiscard]] virtual auto getNodesId() const = 0;

 
};

using ElementShell181 = Shell<4>;

}  // namespace lsdynatoapdl

#endif  // LSDYNA_TO_APDL_SHELL_ELEMENT_H_
