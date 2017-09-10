#include "apdlinterpreter.h"
#include "lsdynainterpreter.h"


void APDLsintax::Writer(QString pfilename, QVector<NodeProperty> pVectorNode, QVector<ShellProperty> pVectorElementShell)
{
    qDebug()<<"Called Writer() function";

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
            qDebug()<< "Element Id:" << pVectorElementShell.at(it).id_element <<", E," << pVectorElementShell.at(it).node1 << ","
                    << pVectorElementShell.at(it).node2 << "," << pVectorElementShell.at(it).node3 << ","
                    << pVectorElementShell.at(it).node4 << "thickness element:" << pVectorElementShell.at(it).thickness;

            //print on file shell element vector
            stream << "SECT," + pVectorElementShell[it].id_element + ",SHELL,," + "\n";
            stream << "SECTDATA, " + pVectorElementShell[it].thickness + ",1,0.0,3"+ "\n";
            stream << "SECOFFEST,MID"  << endl;
            stream << "SECCONTROL,,,, , , ,"<< endl;
            stream << "TYPE,1" << endl;
            stream << "MAT,1"  << endl;
            stream << "REAL,"  << endl;
            stream << "ESYS, 0 SECNUM," + pVectorElementShell[it].id_element + "\n\n";
            stream << "E,"+ pVectorElementShell[it].node1 + "," + pVectorElementShell[it].node2 + "," +
                      pVectorElementShell[it].node3 + "," + pVectorElementShell[it].node4 + "\n\n\n";
        }

        qDebug()<<"End make element shell command APDL!";
    }

    //close file
    file.close();
    qDebug()<<"closed file...";
}
