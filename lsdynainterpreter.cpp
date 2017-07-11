#include "lsdynainterpreter.h"

LsDynaSintax::Node::Node()
{
    //set pattern for search scheme of node definition
    re.setPattern("(\\d+) ?\\s*     (-?\\d+\\.+\\d+)  ?\\s*   (-?\\d+\\.+\\d+)   ?\\s*   (-?\\d+\\.+\\d+)");
    //clean teh vector
    _OutputNode.clear();
}

void LsDynaSintax::Node::setNodeReader(QString pInputFile)
{
    _match = re.match(pInputFile);
    qDebug()<<"fonud groups:" << re.captureCount();
    if (_match.hasMatch())
    {
        //capture id node = number's node
        _IDnode = _match.captured(1);
        //capture coordinate x of node
        _CoordinateNodeX= _match.captured(2);
        //capture coordinate y of node
        _CoordinateNodeY= _match.captured(3);
        //capture coordinate z of node
        _CoordinateNodeZ= _match.captured(4);
        //other field in input string are ignored
        qDebug()<< _IDnode << _CoordinateNodeX << _CoordinateNodeY << _CoordinateNodeZ;
    }

    //convert string in number and store in field struct in vector
    _TempOutputNode.id         = _IDnode.toInt();
    _TempOutputNode.cordinatex = _CoordinateNodeX.toDouble();
    _TempOutputNode.cordinatey = _CoordinateNodeY.toDouble();
    _TempOutputNode.cordinatez = _CoordinateNodeZ.toDouble();
    _OutputNode.append(_TempOutputNode);
}

QVector<NodeProperty> LsDynaSintax::Node::getNodeStructure() {return _OutputNode;}

LsDynaSintax::Node::~Node()
{
    _OutputNode.clear();
}
