#include "writeapdl.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>


void writeAPDL(QString FileName, Node *node, Shell *shell)
{
    qDebug() << "Call writeAPDL()";
    QFile file( FileName );
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
        qDebug() << header;
        // wtite node section
        stream << "! >>>>> node definition << << <\n\n";
        QString nodeoutput;
        for(int it = 1; it < node->size(); ++it)
        {
            nodeoutput.clear();
            //node string N, command to define node ',' is field separetor
            nodeoutput = "N," + QString("%1").arg(node->getIdNode(it)) + ',';     //set number of node
            nodeoutput += QString::number(node->getCoordinateX(it), 'f', 10) + ',';  //set coordinate x
            nodeoutput += QString::number(node->getCoordinateY(it), 'f', 10) + ',';  //set coordinate y
            nodeoutput += QString::number(node->getCoordinateZ(it), 'f', 10) + '\n'; //set coordinate z
            qDebug() << nodeoutput;
            stream << nodeoutput;
            nodeoutput.clear(); //reset the string
        }
        stream << "\n\n! >>>>> end node definition << << <\n\n";
        //declare section element
        stream << "\n\n! >>>>> element shell element << << <\n\n";
        QString elem;
        for(int it = 0; it < shell->size() ; it++)
        {
            elem.clear();
            //print on file shell element vector
            stream << "SECT," + QString::number( shell->getElementID(it)) + ",SHELL,," + "\n";
            stream << "SECTDATA, " + QString::number( shell->getElementThickness(it), 'f', 10) + ",1,0.0,3"+ "\n";
            stream << "SECOFFEST,MID"  << endl;
            stream << "SECCONTROL,,,, , , ," << endl;
            stream << "TYPE,1" << endl;
            stream << "MAT,1"  << endl;
            stream << "REAL,"  << endl;
            stream << "ESYS, 0 SECNUM," + QString::number( shell->getElementID(it)) + "\n\n";
            //print element 4 node
            elem = "E," + QString::number( shell->getElementNode(it, 0)) + ',';
            elem += QString::number( shell->getElementNode(it, 1)) + ',' + QString::number( shell->getElementNode(it, 2)) + ',';
            elem += QString::number( shell->getElementNode(it, 3)) + "\n\n";
            qDebug() << elem;
            stream << elem << endl;
        }
        stream << "\n\n! >>>>> end element definition << << <\n\n";
        file.close(); //close file
    }
}
