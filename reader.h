#ifndef READER_H
#define READER_H
#include <QFile>
#include <QDebug>
#include <QString>
#include <QtConcurrent>
#include <QMessageBox>
#include "convertersintax.h"

void read(QString pFileName, ConverterSintaX* pConverter, LsDynaSintax::Node *pNode, LsDynaSintax::ElementShell *pShell);

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
