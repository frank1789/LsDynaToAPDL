// #include "writeapdl.h"

// #include <QDebug>
// #include <QFile>
// #include <QTextStream>

// void writeAPDL(const QString &FileName, Node *node, Shell *shell) {
//   qDebug() << "start function writeAPDL()";
//   QFile file(FileName);
//   if (file.open(QIODevice::ReadWrite)) {
//     QTextStream stream(&file);
//     // make header file general command Ansys
//     stream << "FINISH"
//            << "\n"
//            << "/CLEAR,START,NEW"
//            << "\n\n";
//     // enter prepocessing section
//     stream << "/PREP7"
//            << "\n\n";
//     // wtite node section
//     stream << "! >>>>> node definition << << <\n\n";
//     // clang-format off
//         QString nodeoutput {""};
//         for(int it = 1; it < node->size(); ++it)
//         {
//             nodeoutput.clear();
//             //node string N, command to define node ',' is field separetor
//             nodeoutput = "N," + QString("%1").arg(node->getIdNode(it)) + ',';
//             //set number of node nodeoutput +=
//             QString::number(node->getCoordinateX(it), 'f', 10) + ',';  //set
//             coordinate x nodeoutput +=
//             QString::number(node->getCoordinateY(it), 'f', 10) + ',';  //set
//             coordinate y nodeoutput +=
//             QString::number(node->getCoordinateZ(it), 'f', 10) + '\n'; //set
//             coordinate z qDebug() << nodeoutput; stream << nodeoutput;
//             nodeoutput.clear(); //reset the string
//         }

//         stream << "\n\n! >>>>> end node definition << << <\n\n";
//         //declare section element
//         stream << "\n\n! >>>>> elements shell element << << <\n\n";
//         QString elem;
//         for(int it = 0; it < shell->size() ; it++)
//         {
//             elem.clear();
//             //print on file shell element vector
//             stream << "SECT," + QString::number( shell->getElementID(it)) +
//             ",SHELL,," + "\n"; stream << "SECTDATA, " + QString::number(
//             shell->getElementThickness(it), 'f', 10) + ",1,0.0,3"+ "\n";
//             stream << "SECOFFEST,MID"  << Qt::endl;
//             stream << "SECCONTROL,,,, , , ," << Qt::endl;
//             stream << "TYPE,1" << Qt::endl;
//             stream << "MAT,1"  << Qt::endl;
//             stream << "REAL,"  << Qt::endl;
//             stream << "ESYS, 0 SECNUM," + QString::number(
//             shell->getElementID(it)) + "\n\n";
//             //print element 4 node
//             elem = "E," + QString::number( shell->getElementNode(it, 0)) +
//             ','; elem += QString::number( shell->getElementNode(it, 1)) + ','
//             + QString::number( shell->getElementNode(it, 2)) + ','; elem +=
//             QString::number( shell->getElementNode(it, 3)) + "\n\n"; qDebug()
//             << elem; stream << elem << Qt::endl;
//         }
//     // clang-format on
//     stream << "\n\n! >>>>> end elements definition << << <\n\n";
//     file.close();  // close file
//   }
// }
