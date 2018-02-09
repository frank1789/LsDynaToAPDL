#ifndef READER_H
#define READER_H
#include "QString"
#include "convertersintax.h"
#include "node.h"
#include "shell.h"

void read(QString pFileName, ConverterSintaX* pConverter, Node *Node, Shell *Shell);

class ManageFile
{
public:
  ManageFile();
  ManageFile(QString pfileName);
  void setFile(QString pfileName);
  void setNewfileName();
  QString getNewfileName();
  QString getfileName();
  double getSizeInfo();
private:
  QString _fileName;
  QString _NewfileName;
  QString _replace;
  double sizeFile;
};


#endif // READER_H
