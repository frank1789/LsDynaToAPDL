#ifndef ELEMENT_SHELL_FOUR_NODE_H
#define ELEMENT_SHELL_FOUR_NODE_H

#include "elementproperty.h"

constexpr int kFourNode{4};

class ShellFourNode : public Element {
 public:
  /**
   * @brief Construct a new Shell Four Node object
   * 
   */
  explicit ShellFourNode() noexcept;

  /**
   * @brief Destroy the Shell Four Node object
   * 
   */
  ~ShellFourNode() override = default;

  /**
   * @brief clone method get a clone of an Element Shell Four Node Object
   * 
   * @return std::unique_ptr<Element> 
   */
  std::unique_ptr<Element> clone() override;

  /**
   * @brief parseElement method filter data about element line by line
   * 
   * @param[in] inputline stream that contains node 
   */
  void parseElement(const QString& inputline) override;

  /**
   * @brief Set the Id object
   * 
   * @param id number that identify the number of the element
   */
  void setId(quint64 id) override;

  /**
   * @brief Set the Nodes object
   * 
   * @param li numbers that identify the nodes that compose the element
   */
  void setNodes(std::initializer_list<quint64> const& li) override;

  /**
   * @brief Set the Thickness object
   * 
   * @param thickness number that specify the thickness'element
   */
  void setThickness(qreal thickness) override;

  /**
   * @brief Get the Id object
   * 
   * @return quint64 
   */
  [[nodiscard]] quint64 getId() const override;

  /**
   * @brief Get the Thickness object
   * 
   * @return qreal 
   */
  [[nodiscard]] qreal getThickness() const override;

  /**
   * @brief Get the Nodes Id object
   * 
   * @return QVector<quint64> 
   */
  [[nodiscard]] QVector<quint64> getNodesId() const override;

  template <class T>
  friend T& operator<<(T& os, const ShellFourNode& s);

 private:
  bool node_flag_;      /**< confirm extraction of node's connection. */
  bool thickness_flag_; /**< confirm extraction shell's thickness. */
};

template <class T>
inline T& operator<<(T& os, const ShellFourNode& s);

template <>
inline std::ostream& operator<<(std::ostream& os, const ShellFourNode& s) {
  os << "element:\n\t";
  os << "id: " << std::setw(15) << s.getId() << "\t";
  auto nodes = s.getNodesId();
  for (auto i = 0; i < nodes.size(); i++) {
    os << "node " << i + 1 << ":" << std::setw(15) << nodes[i];
  }
  os << "\n";
  os << "thickness: " << std::setprecision(7) << s.getThickness() << "\n";
  return os;
}

template <>
inline QDebug& operator<<(QDebug& os, const ShellFourNode& s) {
  os << "element:\n\t";
  os << "id: " << s.getId() << "\t";
  auto nodes = s.getNodesId();
  for (auto i = 0; i < nodes.size(); i++) {
    os << "node " << i + 1 << ":" << nodes[i];
  }
  os << "\n";
  os << "thickness: " << s.getThickness() << "\n";
  return os;
}

#endif  // ELEMENT_SHELL_FOUR_NODE_H
