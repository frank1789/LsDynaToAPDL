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
    ManageFile(QString pFilename);

    void setnewname();

    QString getnewname();

    void setinfo();

    double getsize();

private:
    QString fileName, replace;

    double sizeFile;
};


#endif // READER_H
