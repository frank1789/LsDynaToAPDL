#include "node.h"

#include <QDebug>
#include <QRegularExpression>



/**
 * @brief Node::Node default constructor.
 */
Node::Node()
{
}

/**
 * @brief Node::~Node destructor.
 */
Node::~Node()
{
}

/**
 * @brief Node::readfromfile implementation to import the nodes from the string.
 * @details through regular expressions, it divides the input string from the
 * ralive data to the node and adds them to the vector.
 * @param pInputLine: line input from origin file.
 */
void Node::readfromfile(const QString &inputline)
{
    QRegularExpression re;
    //  set pattern for search scheme of node definition
    // extract coordinate z from string ------------------------------------------------+
    // extract coordinate y from string -----------------------+                        |
    // extract coordinate x from string ------+                |                        |
    // extract node id ---+                   |                |                        |
    //                    +                   +                +                        +
    re.setPattern("(\\d+) ?\\s*   (-?\\d+\\.+\\d+)?\\s*(-?\\d+\\.+\\d+)?\\s*(-?\\d+\\.+\\d+)");
    QRegularExpressionMatch match = re.match(inputline);
    qDebug() << "fonud groups:" << re.captureCount();
    if (match.hasMatch())
    {
      data_.setId_node(match.captured(1).toInt());      //capture id node = number's node
        data_.setCoordinate_x(match.captured(2).toDouble());  //capture coordinate x of node
        data_.setCoordinate_y(match.captured(3).toDouble());  //capture coordinate y of node
        data_.setCoordinate_z(match.captured(4).toDouble());  //capture coordinate z of node
        //other field in input string are ignored
        qDebug() << data_;
    }
    //fill with node extracted
//    _nodeimport->push_back(data_);
}

///**
// * @brief Node::getIdNode return the IDs node.
// * @param i position of node in the vector.
// * @return IDs node
// */
//int Node::getIdNode(int i) {return _nodeimport->at(i).idnode;}

///**
// * @brief Node::getCoordinateX return the x coordinate.
// * @param i position of node in the vector.
// * @return x coordinate.
// */
//double Node::getCoordinateX(int i) {return _nodeimport->at(i).coordinate_x;}

///**
// * @brief Node::getCoordinateZ return the y coordinate
// * @param i position of node in the vector.
// * @return y coordinate.
// */
//double Node::getCoordinateY(int i) {return _nodeimport->at(i).coordinate_y;}

///**
// * @brief Node::getCoordinateZ return the x coordinate
// * @param i position of node in the vector.
// * @return z coordinate.
// */
//double Node::getCoordinateZ(int i) {return _nodeimport->at(i).coordinate_z;}

///**
// * @brief Node::size return dimension of node vector.
// * @return dimension of vector.
// */
//long Node::size() {return _nodeimport->size();}
