#ifndef READER_H
#define READER_H
#include <QFile>
#include <QDebug>
#include <QString>
#include <QtConcurrent>
#include <QMessageBox>
#include "convertersintax.h"

void read(QString pFileName, ConverterSintaX* pConverter, LsDynaSintax::Node *pNode, LsDynaSintax::ElementShell *pShell);

#endif // READER_H
