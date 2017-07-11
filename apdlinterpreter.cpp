#include "apdlinterpreter.h"
#include "lsdynainterpreter.h"

APDLsintax::Node::Node(QVector<NodeProperty> pvectornode)
{
    _NodeToWrite = pvectornode;
    _NodeStringOutput.clear();
    //make header Apdl File command
    _header = "FINISH\n";
    _header += "/CLEAR,START,NEW\n\n";
    _header += "/PREP7\n\n";

}

void APDLsintax::Node::setNodeWriter()
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
    _NodeStringOutput += _NodeToWrite[it].cordinatez +'\n';
    qDebug()<<_NodeStringOutput;
    }
}
