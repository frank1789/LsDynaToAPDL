#include "node.h"

#include <QDebug>
#include <QRegularExpression>

#include "logger_tools.h"

/**
 * @brief Node::parseNode implementation to import the nodes from the string.
 * @details through regular expressions, it divides the input string from the
 * ralive data to the node and adds them to the vector.
 * @param pInputLine: line input from origin file.
 */
void Node::parseNode(const QString &inputline)
{
    QRegularExpression re;
    // set pattern for search scheme of node definition
    // extract coordinate z from string ------------------------------------------------+
    // extract coordinate y from string -----------------------+                        |
    // extract coordinate x from string ------+                |                        |
    // extract node id ---+                   |                |                        |
    //                    +                   +                +                        +
    re.setPattern(R"(\\d+) ?\\s*   (-?\\d+\\.+\\d+)?\\s*(-?\\d+\\.+\\d+)?\\s*(-?\\d+\\.+\\d+)");
    QRegularExpressionMatch match = re.match(inputline);
    qDebug() << INFOFILE << "fonud groups:" << re.captureCount();
    if (match.hasMatch())
    {
      data_.setId_node(match.captured(1).toInt());      //capture id node = number's node
      data_.setCoordinate_x(match.captured(2).toDouble());  //capture coordinate x of node
      data_.setCoordinate_y(match.captured(3).toDouble());  //capture coordinate y of node
      data_.setCoordinate_z(match.captured(4).toDouble());  //capture coordinate z of node
      //other field in input string are ignored
      qDebug() << INFOFILE << data_;
    }
    // fill with node extracted
//    _nodeimport->push_back(data_);
}
