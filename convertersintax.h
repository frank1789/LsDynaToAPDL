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
    //virtual void LsDynaSintax() = 0;
    //virtual void APDLSintax() = 0;
   ConverterSintaX();
   ~ConverterSintaX();
   void setInputLine(QString p_linefile, LsDynaSintax::Node *ptest);
   int test(QString p_linefile,LsDynaSintax::Node *ptest);
private:
   int _LineNumber;
   QVector<QString> _InputLine;
   LsDynaSintax::KeywordDyna _mode;
};

#endif // CONVERTERSINTAX_H


/*
void HeaderApdl()
{
    QString header = "FINISH\n";
    header += "/CLEAR,START,NEW\n\n";
    header += "/PREP7\n\n";
}

void MakeElemenType()
{
    QString elementtype = "ET,";
    elementtype += QString(idelement);
    elementtype += QString(typeelement) + ",";
}

void MakeElement()
{


}
*/
