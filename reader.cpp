#include "reader.h"

void read(QString pFileName, ConverterSintaX* pConverter, LsDynaSintax::Node *pNode, LsDynaSintax::ElementShell *pShell)
{
    qDebug()<<"Call function reader in thread" << QThread::currentThreadId() << "get inputfile" << pFileName;;

    //read file
    QFile file(pFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        pConverter->setInputLine(line, pNode, pShell);
    }

    file.close();
}
