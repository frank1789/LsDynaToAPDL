#include "reader.h"
#include <QFile>

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

ManageFile::ManageFile(QString pFilename)
{
    fileName = pFilename;
}

void ManageFile::setnewname()
{
    //extract name
    QRegularExpression re("(\\w+.\\w+)$");
    re.match(fileName);
    replace = "ApdlConverted.txt";
    qDebug() << fileName.replace(re, replace);
}

QString ManageFile::getnewname()
{
    return fileName;
}

void ManageFile::setinfo()
{
    QFile inputFile(fileName);

    //update group information file
    sizeFile = inputFile.size()/(1048576);
}

double ManageFile::getsize()
{
    ManageFile::setinfo();
    return sizeFile;
}
