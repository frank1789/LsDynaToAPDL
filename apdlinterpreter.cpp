#include "apdlinterpreter.h"
#include "lsdynainterpreter.h"
/*
APDLsintax::ApdlInterpreter::~ApdlInterpreter()
{
    qDebug() << "Pure virtual destructor is called for ApdlInterpreter()";
}

APDLsintax::Node::Node(QVector<NodeProperty> pVectorNode)
{
    qDebug()<<"Call constructor Node()";
    _NodeToWrite = pVectorNode;
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
    qDebug()<<"Called ~Node()";

}

APDLsintax::ElementShell::ElementShell()
{
    qDebug() << "Call constructor ElementShell()";
    a = 0;
}

void APDLsintax::ElementShell::setWriter()
{
    qDebug() << "success!";

    sect,1229203,shell,,
    secdata,3.734283,1,0.0,3
    secoffset,MID
    seccontrol,,,, , , ,

    TYPE, 1
    MAT, 1
    REAL,
    ESYS, 0 SECNUM, 1229203
    E,1189369,1188878,1188879,1189370


    "SECT," + id + ",SHELL,,";
    "SECTDATA, " + thickness + ",1,0.0,3";
    "SECOFFEST,MID"
    "SECCONTROL,,,, , , ,"
    "TYPE,1
    "MAT,1
    "REAL,"
    "ESYS, 0 SECNUM," + id
    "E,"+ node1+ "," + node2+ ","+ node3+","+node4

}

APDLsintax::ElementShell::~ElementShell()
{
    qDebug() <<"Called ~ElementShell()";
}
*/
void APDLsintax::Writer(QString pfilename, QVector<NodeProperty> pVectorNode, QVector<ShellProperty> pVectorElementShell)
{
    qDebug()<<"Called Writer()";

    QFile file( pfilename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        //make header file general command Ansys
        QString header = "FINISH\n";
        header += "/CLEAR,START,NEW\n\n";

        //enter prepocessing section
        header += "/PREP7\n\n";

        //write on file
        stream << header;
        qDebug()<<header;
        //comment on file to separe section
        stream << "!Declare node\n\n";

        //write node
        QString NodeStringOutput;
        for(int it = 1; it < pVectorNode.size(); ++it)
        {
            //node string N, command to define node ',' is field separetor
            NodeStringOutput = "N,";

            //set number of node
            NodeStringOutput += pVectorNode[it].id + ',';

            //set coordinate x
            NodeStringOutput += pVectorNode[it].cordinatex + ',';

            //set coordinate y
            NodeStringOutput += pVectorNode[it].cordinatey + ',';

            //set coordinate z
            NodeStringOutput += pVectorNode[it].cordinatez + '\n';
            qDebug()<<NodeStringOutput;
            stream <<NodeStringOutput;
            NodeStringOutput.clear();
        }
        qDebug()<<"End make node command APDL!";

        //comment on file to separe section
        stream << "\n\n!Declare element shell element\n\n";

        //write element
        for(int it = 1; it < pVectorElementShell.size() ; ++it)
        {
            stream << "SECT," + pVectorElementShell[it].id_element + ",SHELL,," + "\n";
            stream << "SECTDATA, " + pVectorElementShell[it].thickness + ",1,0.0,3"+ "\n";
            stream << "SECOFFEST,MID"  << endl;
            stream << "SECCONTROL,,,, , , ,"<< endl;
            stream << "TYPE,1" << endl;
            stream << "MAT,1"  << endl;
            stream << "REAL,"  << endl;
            stream << "ESYS, 0 SECNUM," + pVectorElementShell[it].id_element + "\n\n";
            stream << "E,"+ pVectorElementShell[it].node1 + "," + pVectorElementShell[it].node2 + "," + pVectorElementShell[it].node3 + "," + pVectorElementShell[it].node4 + "\n\n\n";
        }
        qDebug()<<"End make element shell command APDL!";
    }

    //close file
    file.close();
    qDebug()<<"closed file...";
}
