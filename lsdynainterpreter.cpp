#include "lsdynainterpreter.h"

LsDynaSintax::LsDynaInterpreter::~LsDynaInterpreter()
{
    qDebug() << "Pure virtual destructor is called for LsDynaInterpreter";
}

LsDynaSintax::Node::Node()
{
    //set pattern for search scheme of node definition
    //extract coordinate z from string -----------------------------------------------------------+
    //extract coordinate y from string ----------------------------------+                        |
    //extract coordinate x from string --------+                         |                        |
    //extract node id ---+                     |                         |                        |
    //                   +                     +                         +                        +
    _re.setPattern("(\\d+) ?\\s*     (-?\\d+\\.+\\d+)  ?\\s*   (-?\\d+\\.+\\d+)   ?\\s*   (-?\\d+\\.+\\d+)");

    //clean the vector
    _OutputNode.clear();
}

void LsDynaSintax::Node::setReader(QString pInputFile)
{
    _match = _re.match(pInputFile);
    qDebug()<<"fonud groups:" << _re.captureCount();
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
    _TempOutputNode.id         = _IDnode;
    _TempOutputNode.cordinatex = _CoordinateNodeX;
    _TempOutputNode.cordinatey = _CoordinateNodeY;
    _TempOutputNode.cordinatez = _CoordinateNodeZ;
    _OutputNode.append(_TempOutputNode);
}

QVector<NodeProperty> LsDynaSintax::Node::getNodeStructure() {return _OutputNode;}

LsDynaSintax::Node::~Node()
{
    //clear vector of node
    _OutputNode.clear();
    qDebug() <<"Called ~Node()";
}

LsDynaSintax::ElementShell::ElementShell()
{
    qDebug()<<a;
    _re.setPattern("(\\d+)       (\\d+) (\\d+) (\\d+) (\\d+) (\\d+)");
}

void LsDynaSintax::ElementShell::setReader(QString pInputFile)
{
    _match = _re.match(pInputFile);
    qDebug()<<"fonud groups:" << _re.captureCount();
}

LsDynaSintax::ElementShell::~ElementShell()
{
    qDebug()<<"Call ~ElementShell";
}
