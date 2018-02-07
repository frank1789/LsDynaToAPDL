#include "node.h"
#include "QDebug"
#include "QRegularExpression"


Node::Node() {
  _nodeimport = new QVector<propnode<int, double>> {};
}


Node::~Node() {
  delete _nodeimport;
}


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
    _nodeimport->append(_datain);
}



int Node::getIdNode() {return _datain.idnode;}

double Node::getCoordinateX() {return _datain.coordinate_x;}

double Node::getCoordinateY() {return _datain.coordinate_y;}

double Node::getCoordinateZ() {return _datain.coordinate_z;}

