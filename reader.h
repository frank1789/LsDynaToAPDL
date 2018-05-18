#ifndef READER_H
#define READER_H
#include <QString>
#include "convertersintax.h"
#include "node.h"
#include "shell.h"


void read(QString pFileName, ConverterSintax* pConverter, Node *node, Shell *Shell);



#endif // READER_H
