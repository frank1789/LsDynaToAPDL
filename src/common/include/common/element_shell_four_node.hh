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

inline constexpr int kFourNode{4};

class ShellFourNode : public Shell<ShellFourNode> {
  friend class Element<Shell<ShellFourNode>>;
 public:
  explicit ShellFourNode() noexcept;

  void parse_element(std::string_view &input_line);

  //   /**
  //    * @brief Destroy the Shell Four Node object
  //    *
  //    */
  //   ~ShellFourNode() override = default;

  //   /**
  //    * @brief clone method get a clone of an Element Shell Four Node Object
  //    *
  //    * @return std::unique_ptr<Element>
  //    */
  //   std::unique_ptr<Element> clone() override;

  //   /**
  //    * @brief parseElement method filter data about element line by line
  //    *
  //    * @param[in] inputline stream that contains node
  //    */
  //   void parseElement(const QString &inputline) override;

  //   /**
  //    * @brief Set the Id object
  //    *
  //    * @param id number that identify the number of the element
  //    */
  //   void setId(uint64_t id) override;

  //   /**
  //    * @brief Set the Nodes object
  //    *
  //    * @param li numbers that identify the nodes that compose the element
  //    */
  //   void setNodes(std::initializer_list<uint64_t> const &li) override;

  //   /**
  //    * @brief Set the Thickness object
  //    *
  //    * @param thickness number that specify the thickness'element
  //    */
  //   void setThickness(double thickness) override;

  //   /**
  //    * @brief Get the Id object
  //    *
  //    * @return uint64_t
  //    */
  //   [[nodiscard]] uint64_t getId() const override;

  //   /**
  //    * @brief Get the Thickness object
  //    *
  //    * @return double
  //    */
  //   [[nodiscard]] double getThickness() const override;

  //   /**
  //    * @brief Get the Nodes Id object
  //    *
  //    * @return QVector<uint64_t>
  //    */
  //   [[nodiscard]] QVector<uint64_t> getNodesId() const override;

  //   friend QDebug &operator<<(QDebug &os, const ShellFourNode &s);

 private:
  bool node_flag_;      /**< confirm extraction of node's connection. */
  bool thickness_flag_; /**< confirm extraction shell's thickness. */
};

// QDebug &operator<<(QDebug &os, const ShellFourNode &s);

using ElementShell181 = ShellFourNode;

#endif  // LSDYNA_TO_APDL_COMMON_ELEMENT_SHELL_FOUR_NODE_HH
