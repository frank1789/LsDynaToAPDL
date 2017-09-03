#include "apdlinterpreter.h"
#include "lsdynainterpreter.h"

APDLsintax::ApdlInterpreter::~ApdlInterpreter()
{
    qDebug() << "Pure virtual destructor is called for ApdlInterpreter()";
}

APDLsintax::Node::Node(QVector<NodeProperty> pvectornode)
{
    _NodeToWrite = pvectornode;
    _NodeStringOutput.clear();
    //make header Apdl File command
    _header = "FINISH\n";
    _header += "/CLEAR,START,NEW\n\n";
    _header += "/PREP7\n\n";

}

void APDLsintax::Node::setWriter()
{
    for(int it = 0; it < _NodeToWrite.size(); it++)
    {
        //node string N, command to define node ',' is field separetor
        _NodeStringOutput = "N,";

        //set number of node
        _NodeStringOutput += _NodeToWrite[it].id + ',';

        //set coordinate x
        _NodeStringOutput += _NodeToWrite[it].cordinatex + ',';

        //set coordinate y
        _NodeStringOutput += _NodeToWrite[it].cordinatey + ',';

        //set coordinate z
        _NodeStringOutput += _NodeToWrite[it].cordinatez + '\n';
        qDebug()<<_NodeStringOutput;
    }
    qDebug()<<"End make node command APDL!";
}

APDLsintax::Node::~Node()
{
    qDebug()<<"Call ~Node()";

}

APDLsintax::ElementShell::ElementShell()
{
     qDebug() << "success!";
}

void APDLsintax::ElementShell::setWriter()
{
    qDebug() << "success!";
}
