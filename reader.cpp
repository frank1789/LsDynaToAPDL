#include "reader.h"
#include "QDebug"
#include "QFile"
#include "QRegularExpression"
#include "QThread"
#include "QMessageBox"

//template<typename N, typename E>
void read(QString pFileName, ConverterSintaX* pConverter, Node *Node, Shell *Shell)
{
  qDebug()<<"Call function reader in thread" << QThread::currentThreadId();
  qDebug()<< "get inputfile" << pFileName;;
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
      pConverter->setInputLine(line, Node, Shell);
    }

  file.close();
}


ManageFile::ManageFile()
{
  _fileName.clear();
  _NewfileName.clear();
  _replace.clear();
}

ManageFile::ManageFile(QString pfileName)
{
  _fileName = pfileName;
}

void ManageFile::setFile(QString pfileName)
{
  _fileName = pfileName;
}

void ManageFile::setNewfileName()
{
  _NewfileName = _fileName;
  //extract name
  QRegularExpression re("(\\w+.\\w+)$");
  re.match(_NewfileName);
  _replace = "ApdlConverted.txt";
  qDebug() << _NewfileName.replace(re, _replace);
}

QString ManageFile::getNewfileName() {return _NewfileName;}

QString ManageFile::getfileName() {return _fileName;}


double ManageFile::getSizeInfo()
{
  QFile inputFile(_fileName);
  sizeFile = inputFile.size()/(1048576); //update group information file
  return sizeFile;
}
