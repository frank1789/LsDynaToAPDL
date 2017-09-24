#ifndef CONVERTERSINTAX_H
#define CONVERTERSINTAX_H
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFile>
#include <QRegularExpression>
#include "lsdynainterpreter.h"
#include "apdlinterpreter.h"

class ConverterSintaX
{
public:
   ConverterSintaX();
   ~ConverterSintaX();
   void setInputLine(QString p_linefile, LsDynaSintax::Node *pNode, LsDynaSintax::ElementShell *pShell);
   int test(QString p_linefile, LsDynaSintax::Node *pNode, LsDynaSintax::ElementShell *pShell);
private:
   int _LineNumber;
   QVector<QString> _InputLine;
   LsDynaSintax::KeywordDyna _mode;
};

#endif // CONVERTERSINTAX_H
