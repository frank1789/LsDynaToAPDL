#ifndef ELEMENT_SHELL_FOUR_NODE_H
#define ELEMENT_SHELL_FOUR_NODE_H

#include "elementproperty.h"

constexpr int kFourNode{4};

class ShellFourNode : public PropertyElement {
 public:
  explicit ShellFourNode();

  ~ShellFourNode() override = default;

  void parseElement(const QString& inputline) override;

  void setId(quint64 id) override;
  void setNodes(std::initializer_list<quint64> const& li) override;

  void setThickness(qreal thickness) override;

  quint64 getId() const override;
  qreal getThickness() const override;

  QVector<quint64> getNodesId() const override;

  template <typename TR>
  TR getElements() const {
    return shells_;
  }

  int size() const override;

 private:
  QVector<ShellFourNode> shells_;
  bool node_flag_;      /**< confirm extraction of node's connection. */
  bool thickness_flag_; /**< confirm extraction shell's thickness. */
};

std::ostream& operator<<(std::ostream& os, const ShellFourNode& s);

QDebug& operator<<(QDebug& os, const ShellFourNode& s);

#endif  // ELEMENT_SHELL_FOUR_NODE_H
