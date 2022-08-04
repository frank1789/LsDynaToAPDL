#include "element_shell_four_node.h"

#include <QDebug>
#include <QRegularExpression>

#include "logger_tools.h"

ShellFourNode::ShellFourNode() noexcept :
    Element(), node_flag_(false), thickness_flag_(false) {
  element_id_.reserve(kFourNode);
}

/**
 * @brief Implementation to import the information of the
 * shell elements from the source string where it is saved on two lines.
 *
 * @details For this reason, the function extracts information regarding the
 * unique identifier of the element number and the nodes with which it is
 * created, in this specific application there is a 4-node element.
 * If the extraction is successful, the flag on nodes assumes true Boolean
 * value. Thus the information of the shell shell replicate thickness is
 * extracted if constant. A second flag assumes true value, so everything is
 * saved in class
 *
 * @param[in] inputline: line from origin file.
 *
 */
void ShellFourNode::parseElement(const QString &inputline) {
  QRegularExpression re;
  // clang-format off
  //  set pattern for search scheme of element definition
  //  extract node element 4 from string -----------------------------------------------------------------+
  //  extract node element 3 from string -------------------------------------------------+               |
  //  extract node element 2 from string --------------------------------+                |               |
  //  extract node element 1 from string ---------------+                |                |               |
  //  extract element card ignored -----+               |                |                |               |
  //  extract id -------+               |               |                |                |               |
  //                    +               +               +                +                +               +
  re.setPattern(R"((?<id>\d+)\s+(?<card>\d+)\s(?<node1>\d+)\s(?<node2>\d+)\s(?<node3>\d+)\s(?<node4>\d+))");
  // clang-format on
  // verify captured groups
  QRegularExpressionMatch match = re.match(inputline);
  if (match.hasMatch()) {
    qDebug().noquote() << INFOFILE << "has match: " << match.hasMatch()
                       << ", fonud groups:" << re.captureCount();
    // capture id element
    setId(static_cast<quint64>(match.captured("id").toInt()));
    auto first_node = static_cast<quint64>(match.captured("node1").toInt());
    auto second_node = static_cast<quint64>(match.captured("node2").toInt());
    auto third_node = static_cast<quint64>(match.captured("node3").toInt());
    auto fourth_node = static_cast<quint64>(match.captured("node4").toInt());
    setNodes({first_node, second_node, third_node, fourth_node});
    node_flag_ = true;
  } else {
    qWarning().noquote()
        << INFOFILE << "WARNING - invalid regex, could be lose information\n\t"
        << inputline;
  }
  // verify second line string element thickeness replicated four times costant
  // for element
  // clang-format off
  re.setPattern(R"((\d+?.\d+)       (\d+?.\d+)       (\d+?.\d+)       (\d+?.\d+))");
  // clang-format on
  match = re.match(inputline);
  if (match.hasMatch()) {
    // clang-format off
    qDebug().noquote() << INFOFILE 
            << "has match: " << match.hasMatch()
            << ", fonud groups:" 
            << re.captureCount();
    // clang-format on
    setThickness(match.captured(1).toDouble());
    qDebug().noquote() << INFOFILE << "thickness element:" << getThickness();
    thickness_flag_ = true;
  } else {
    qWarning().noquote()
        << INFOFILE << "WARNING - invalid regex, could be lose information\n\t"
        << inputline;
  }
  //  fill vector element then reset the flags
  if (node_flag_ == true && thickness_flag_ == true) {
    node_flag_ = false;
    thickness_flag_ = false;
  }
}

void ShellFourNode::setId(quint64 id) { id_ = id; }

void ShellFourNode::setNodes(const std::initializer_list<quint64> &li) {
  if (element_id_.size() == kFourNode) {
    element_id_.clear();
  }

  for (auto e : li) {
    element_id_.push_back(e);
  }
}

void ShellFourNode::setThickness(qreal thickness) { thickness_ = thickness; }

quint64 ShellFourNode::getId() const { return id_; }

qreal ShellFourNode::getThickness() const { return thickness_; }

QVector<quint64> ShellFourNode::getNodesId() const { return element_id_; }

std::unique_ptr<Element> ShellFourNode::clone() {
  return std::make_unique<ShellFourNode>();
}

QDebug &operator<<(QDebug &os, const ShellFourNode &s) {
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
