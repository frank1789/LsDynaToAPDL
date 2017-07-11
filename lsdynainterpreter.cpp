#include "lsdynainterpreter.h"

LsDynaSintax::Node::Node()
{
    re.setPattern("(\\d+) ?\\s*     (-?\\d+\\.+\\d+)  ?\\s*   (-?\\d+\\.+\\d+)   ?\\s*   (-?\\d+\\.+\\d+)");
    _OutputNode.clear();
}

void LsDynaSintax::Node::setNodeReader(QString pInputFile)
{
    _match = re.match(pInputFile);
    qDebug()<<"fonud groups:" << re.captureCount();
    if (_match.hasMatch())
    {
        _IDnode = _match.captured(1);
        _CoordinateNodeX= _match.captured(2);
        _CoordinateNodeY= _match.captured(3);
        _CoordinateNodeZ= _match.captured(4);
        qDebug()<< _IDnode << _CoordinateNodeX << _CoordinateNodeY << _CoordinateNodeZ;
    }

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
