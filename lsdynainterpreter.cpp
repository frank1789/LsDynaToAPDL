#include "lsdynainterpreter.h"

LsDynaSintax::LsDynaInterpreter::~LsDynaInterpreter()
{
    qDebug() << "Pure virtual destructor is called for LsDynaInterpreter()";
}

LsDynaSintax::Node::Node()
{
    //set pattern for search scheme of node definition
    //extract coordinate z from string -----------------------------------------------------------+
    //extract coordinate y from string ----------------------------------+                        |
    //extract coordinate x from string --------+                         |                        |
    //extract node id ---+                     |                         |                        |
    //                   +                     +                         +                        +
    _re.setPattern("(\\d+) ?\\s*   (-?\\d+\\.+\\d+)  ?\\s*   (-?\\d+\\.+\\d+)   ?\\s*   (-?\\d+\\.+\\d+)");

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

    //copy string in field struct in vector
    _TempOutputNode.id         = _IDnode;
    _TempOutputNode.cordinatex = _CoordinateNodeX;
    _TempOutputNode.cordinatey = _CoordinateNodeY;
    _TempOutputNode.cordinatez = _CoordinateNodeZ;
    _OutputNode.append(_TempOutputNode);
}

QVector<NodeProperty> LsDynaSintax::Node::getNodeStructure()
{
    return _OutputNode;
}

LsDynaSintax::Node::~Node()
{
    //clear vector of node
    _OutputNode.clear();
    qDebug() <<"Called ~Node()";
}

LsDynaSintax::ElementShell::ElementShell()
{
    qDebug() << "Called ElementShell()";

    //clear the vector
    _OutputElmentShell.clear();
    _flagNode =false;
    _flagThickness = false;
}

void LsDynaSintax::ElementShell::setReader(QString pInputFile)
{
    //set pattern for search scheme of element definition
    //extract node element 4 from string -----------------------------------------------------------------+
    //extract node element 3 from string -------------------------------------------------+               |
    //extract node element 2 from string --------------------------------+                |               |
    //extract node element 1 from string ---------------+                |                |               |
    //extract element card ignored -----+               |                |                |               |
    //extract id -------+               |               |                |                |               |
    //                  +               +               +                +                +               +
    _re.setPattern("(?<id>\\d+)\\s+(?<card>\\d+)\\s(?<node1>\\d+)\\s(?<node2>\\d+)\\s(?<node3>\\d+)\\s(?<node4>\\d+)");

    //verify captured groups
    _match = _re.match(pInputFile);
    if(_match.hasMatch())
    {
        qDebug()<<"has match: "<<_match.hasMatch() <<", fonud groups:" << _re.captureCount();
        //capture id
        _shell4node.id_element = _match.captured("id");

        //capture node1
        _shell4node.node1 = _match.captured("node1");

        //capture node2
        _shell4node.node2 = _match.captured("node2");

        //capture node3
        _shell4node.node3 = _match.captured("node3");

        //capture node4
        _shell4node.node4 = _match.captured("node4");
        qDebug()<<"form element id:" << _shell4node.id_element <<", E," <<_shell4node.node1 << ","<< _shell4node.node2 << "," << _shell4node.node3 << "," << _shell4node.node4;
    _flagNode=true;
    }

    //verify second linestring
    //element thickeness replicated four time costant for element
    _re.setPattern("(\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)");
    _match = _re.match(pInputFile);
    if(_match.hasMatch())
    {
        qDebug()<<"has match: "<<_match.hasMatch() <<", fonud groups:" << _re.captureCount();
        _shell4node.thickness = _match.captured(1);
        qDebug() << "thickness element:" << _shell4node.thickness;
        _flagThickness = true;
    }

        //store in output vector
    if(_flagNode == true && _flagThickness == true)
    {
        qDebug()<<"inside if id:" << _shell4node.id_element <<", E," <<_shell4node.node1 << ","<< _shell4node.node2 << "," << _shell4node.node3 << "," << _shell4node.node4 << "thickness element:" << _shell4node.thickness;


        _OutputElmentShell.append(_shell4node);

        _flagNode =false;
        _flagThickness = false;

     }
}

QVector<ShellProperty> LsDynaSintax::ElementShell::getElementStructure()
{
    return _OutputElmentShell;
}

LsDynaSintax::ElementShell::~ElementShell()
{
    qDebug()<<"Call ~ElementShell";
    _OutputElmentShell.clear();
}
