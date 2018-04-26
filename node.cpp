#include "node.h"
#include "QDebug"
#include "QRegularExpression"

#define INIT 35000

/**
 * @brief Node::Node default constructor.
 * @details initializes private variables.
 */
Node::Node()
{
  _nodeimport = new QVector<propnode<int, double>> {};
  _nodeimport->clear();
  _nodeimport->reserve(INIT);
  _countnode_ = 0;
}

/**
 * @brief Node::~Node destructor.
 * @details releases memory.
 */
Node::~Node() {
  delete _nodeimport;
}

/**
 * @brief Node::readfromfile implementation to import the nodes from the string.
 * @details through regular expressions, it divides the input string from the
 * ralive data to the node and adds them to the vector.
 * @param pInputLine input string.
 */
void Node::readfromfile(QString pInputLine)
{
  QRegularExpression _re;
  //  set pattern for search scheme of node definition
  //extract coordinate z from string ------------------------------------------------+
  //extract coordinate y from string -----------------------+                        |
  //extract coordinate x from string ------+                |                        |
  //extract node id ---+                   |                |                        |
  //                   +                   +                +                        +
  _re.setPattern("(\\d+) ?\\s*   (-?\\d+\\.+\\d+)?\\s*(-?\\d+\\.+\\d+)?\\s*(-?\\d+\\.+\\d+)");
  QRegularExpressionMatch _match = _re.match(pInputLine);
  qDebug()<<"fonud groups:" << _re.captureCount();
  if (_match.hasMatch())
    {
      _datain.idnode = _match.captured(1).toInt();      //capture id node = number's node
      _datain.coordinate_x = _match.captured(2).toDouble();  //capture coordinate x of node
      _datain.coordinate_y = _match.captured(3).toDouble();  //capture coordinate y of node
      _datain.coordinate_z = _match.captured(4).toDouble();  //capture coordinate z of node
      //other field in input string are ignored
      qDebug() << _datain.idnode << _datain.coordinate_x << _datain.coordinate_y << _datain.coordinate_z;
    }
  //fill with node extracted
  _nodeimport->push_back(_datain);
  _countnode_++;
}

//void

//void Node::CountNode()
//{
//    emit _countnode_;
//}

/**
 * @brief Node::getIdNode return the IDs node.
 * @param i position of node in the vector.
 * @return IDs node
 */
int Node::getIdNode(int i) {return _nodeimport->at(i).idnode;}

/**
 * @brief Node::getCoordinateX return the x coordinate.
 * @param i position of node in the vector.
 * @return x coordinate.
 */
double Node::getCoordinateX(int i) {return _nodeimport->at(i).coordinate_x;}

/**
 * @brief Node::getCoordinateZ return the y coordinate
 * @param i position of node in the vector.
 * @return y coordinate.
 */
double Node::getCoordinateY(int i) {return _nodeimport->at(i).coordinate_y;}

/**
 * @brief Node::getCoordinateZ return the x coordinate
 * @param i position of node in the vector.
 * @return z coordinate.
 */
double Node::getCoordinateZ(int i) {return _nodeimport->at(i).coordinate_z;}

/**
 * @brief Node::size return dimension of node vector.
 * @return dimension of vector.
 */
long Node::size() {return _nodeimport->size();}
