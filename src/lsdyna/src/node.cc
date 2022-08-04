/**
 * @file node.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The node parser.
 * @version 0.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "node.h"

#include <QDebug>
#include <QRegularExpression>

#include "logger_tools.h"

/**
 * @brief Node::parseNode implementation to import the nodes from the string.
 *
 * @details through regular expressions, it divides the input string from the
 * data to the node and adds them to the vector.
 *
 * @param pInputLine: line input from origin file.
 */
PropertyNode<quint64, qreal> Node::parseNode(const QString &inputline) {
  PropertyNode<quint64, qreal> node;
  // clang-format off
    QRegularExpression re;
    // set pattern for search scheme of node definition
    // extract coordinate z from string ------------------------------------------------+
    // extract coordinate y from string -----------------------+                        |
    // extract coordinate x from string ------+                |                        |
    // extract node id ---+                   |                |                        |
    //                    +                   +                +                        +
    re.setPattern(R"((\d+) ?\s*   (-?\d+\.+\d+)?\s*(-?\d+\.+\d+)?\s*(-?\d+\.+\d+))");
    QRegularExpressionMatch match = re.match(inputline);
    qDebug().noquote() << INFOFILE << "fonud groups:" << re.captureCount();
  // clang-format on
  if (match.hasMatch()) {
    // capture id node = number's node
    auto id = static_cast<quint64>(match.captured(1).toInt());
    // capture coordinate x of node
    auto x = static_cast<qreal>(match.captured(2).toDouble());
    // capture coordinate y of node
    auto y = static_cast<qreal>(match.captured(3).toDouble());
    // capture coordinate z of node
    auto z = static_cast<qreal>(match.captured(4).toDouble());
    // other field in input string are ignored
    node = PropertyNode<quint64, qreal>(id, x, y, z);
    qDebug().noquote() << INFOFILE << node;
  }
  return node;
}
