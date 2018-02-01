#include "lsdynainterpreter.h"
#include <QDebug>

using namespace LsDynaSintax;


/**
 * @brief Node::Node
 */
Node::Node()
{
  //initiziliazi node vector and clear
  _node = new QVector<NodeProperty>;
  _node->clear();
  //set pattern for search scheme of node definition
  //extract coordinate z from string ------------------------------------------------+
  //extract coordinate y from string -----------------------+                        |
  //extract coordinate x from string ------+                |                        |
  //extract node id ---+                   |                |                        |
  //                   +                   +                +                        +
  _re.setPattern("(\\d+) ?\\s*   (-?\\d+\\.+\\d+)?\\s*(-?\\d+\\.+\\d+)?\\s*(-?\\d+\\.+\\d+)");
}


/**
 * @brief Node::readfromfile
 * @param pInputLine
 */
void Node::readfromfile(QString pInputLine)
{
  NodeProperty matched;
  _match = _re.match(pInputLine);
  qDebug()<<"fonud groups:" << _re.captureCount();
  if (_match.hasMatch())
    {
      matched.IdNode = _match.captured(1);      //capture id node = number's node
      matched.coordinateX= _match.captured(2);  //capture coordinate x of node
      matched.coordinateY= _match.captured(3);  //capture coordinate y of node
      matched.coordinateZ= _match.captured(4);  //capture coordinate z of node
      //other field in input string are ignored
      qDebug()<<matched.IdNode << matched.coordinateX << matched.coordinateY << matched.coordinateZ;
    }
  //fill vector
  _node->append(matched);
}


/**
 * @brief LsDynaSintax::Node::getNode
 * @return
 */
QVector<NodeProperty>* LsDynaSintax::Node::getNode() { return _node;}


/**
 * @brief LsDynaSintax::Node::Clear
 */
void Node::reset()
{
  // verify the vector is not void then erase
  qDebug() << _node->isEmpty();
  if(!_node->isEmpty())
    {
      _node->clear();
    }
}


/**
 * @brief LsDynaSintax::Node::~Node
 */
LsDynaSintax::Node::~Node()
{
  _node->clear(); //empty vector
  delete _node;
  qDebug() <<"Called ~Node()";
}
