#include "element_shell.h"

#include <QDebug>
#include <QRegularExpression>

#include "logger_tools.h"

constexpr quint64 kMinimumElements{200000};

Shell::Shell() : node_flag_(false), thickness_flag_(false) {
  shell_.reserve(kMinimumElements);
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
void Shell::parseElement(const QString &inputline) {
  shell_t shelldata;
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
  re.setPattern("(?<id>\\d+)\\s+(?<card>\\d+)\\s(?<node1>\\d+)\\s(?<node2>\\d+)\\s(?<node3>\\d+)\\s(?<node4>\\d+)");
  // clang-format on
  // verify captured groups
  QRegularExpressionMatch match = re.match(inputline);
  if (match.hasMatch()) {
    // clang-format off
    qDebug() << INFOFILE 
            << "has match: " 
            << match.hasMatch()
            << ", fonud groups:" 
            << re.captureCount();
    // clang-format on
    // capture id element
    shelldata.setId(match.captured("id").toInt());
    auto first_node = static_cast<quint64>(match.captured("node1").toInt());
    auto second_node = static_cast<quint64>(match.captured("node2").toInt());
    auto third_node = static_cast<quint64>(match.captured("node3").toInt());
    auto fourth_node = static_cast<quint64>(match.captured("node4").toInt());
    shelldata.setNodes({first_node, second_node, third_node, fourth_node});
    qDebug() << INFOFILE << shelldata;
    node_flag_ = true;
  }
  // verify second line string element thickeness replicated four times costant
  // for element
  // clang-format off
  re.setPattern("(\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)");
  // clang-format on
  match = re.match(inputline);
  if (match.hasMatch()) {
    // clang-format off
    qDebug() << INFOFILE 
            << "has match: " << match.hasMatch()
            << ", fonud groups:" 
            << re.captureCount();
    // clang-format on
    shelldata.setThickness(match.captured(1).toDouble());
    qDebug() << INFOFILE << "thickness element:" << shelldata.getThickness();
    thickness_flag_ = true;
  }
  //  fill vector element then reset the flags
  if (node_flag_ == true && thickness_flag_ == true) {
    shell_.push_back(shelldata);
    node_flag_ = false;
    thickness_flag_ = false;
  }
}

/**
 * @brief Shell::getElements
 * @return QVector shell elements
 */
QVector<Shell::shell_t> Shell::getElements() const { return shell_; }

/**
 * @brief Shell::size get the size of vector contains list of elements.
 * @return dimension vector's elements.
 */
int Shell::size() const { return shell_.size(); }
