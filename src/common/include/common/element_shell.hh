/**
 * @file element_shell.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_COMMON_SHELL_ELEMENT_HH
#define LSDYNA_TO_APDL_COMMON_SHELL_ELEMENT_HH

#include "common/abstract_element_shell.hh"
#include "common/base_element_shell.hh"

// /**
//  * @brief The propelem struct defines the general appearance of the
//  parameters.
//  * of an element.
//  *
//  * @details The content of the structure represents the element defined
//  by a
//  * unique ID, by the number of nodes that constitute it. The thickness
//  used for
//  * shell-like elements.
//  * A template formulation is used to make most of the definitions
//  available to
//  * the structure.
//  * It is suggested to use:
//  *  - Integer data types:
//  *      - element IDs;
//  *      - nodes that constitute it as identified by the ID;
//  *      - number of nodes of the element;
//  *  - Float Data type:
//  *      - thickness.
//  *
//  * @tparam N: type for elements IDs
//  * @tparam EN: type for node IDs
//  * @tparam T: type for element's thickness
//  * @tparam size: sets the number of nodes of the element
//  */
// // template <typename N, typename EN, typename T, int size>
// // class ShellElement : public Element<typename N, typename EN> {
// //  private:
// //   T thickness_; /**< thickness of the element. */
// // };

template <typename T>
class Shell : public T, public AbstractElementShell {
 public:
  //   friend class Element<Shell>;

  //  public:
  //   explicit Shell() = default;

  //   Shell(std::uint64_t id, std::vector<std::uint64_t> nodes, double
  //   thickness) {}

  //   ~Shell() noexcept = default;

  //   //   virtual void setId(uint64_t id) = 0;
  //   //   virtual void setNodes(std::initializer_list<uint64_t> const &li) =
  //   0;
  //   //   virtual void setThickness(double thickness) = 0;

  /**
   * @brief Get the id object
   *
   * @return std::uint64_t
   */
  [[nodiscard]] constexpr auto get_id() const -> std::uint64_t {
    return T::get_id();
  }

  /**
   * @brief Get the thickness object
   *
   * @return double
   */
  [[nodiscard]] constexpr auto get_thickness() const -> double {
    return T::get_tickness();
  }

  //     return m_thickness;
  //   }
  //   // [[nodiscard]] virtual auto getNodesId() const = 0;

  //   private:
  //   std::uint64_t m_id{};
  //   std::vector<std::uint64_t> m_nodes{};
  //   double m_thickness{};
};

#endif  // LSDYNA_TO_APDL_COMMON_SHELL_ELEMENT_HH
