// #include "reader.h"
// #include <QDebug>
// #include <QFile>
// #include <QRegularExpression>
// #include <QThread>
// #include <QMessageBox>


// void read(QString pFileName, ConverterSintax *pConverter, Node *node, Shell *shell)
// {
//   qDebug()<<"Call function reader in thread" << QThread::currentThreadId();
//   qDebug()<< "get inputfile" << pFileName;;
//   //read file
//   QFile file(pFileName);
//   if(!file.open(QIODevice::ReadOnly))
//     {
//       QMessageBox::information(0, "error", file.errorString());
//     }

//   QTextStream in(&file);
//   while(!in.atEnd())
//     {
//       QString line = in.readLine();
//       pConverter->setInputLine(line, node, shell);
//     }

//   file.close();
// }

