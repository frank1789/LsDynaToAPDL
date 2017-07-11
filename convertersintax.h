#ifndef CONVERTERSINTAX_H
#define CONVERTERSINTAX_H
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFile>
#include <QRegularExpression>
#include "lsdynainterpreter.h"

namespace LsDyna {
enum KeywordDyna
{
    $,
    KEYWORD,
    NODE,
    ELEMENTSHELL,
    ELEMENTSOLID,
    INITIALSTRAINSOLID,
    INITIALSTRESSSHELL
};
}


class ConverterSintaX
{
public:
    //virtual void LsDynaSintax() = 0;
    //virtual void APDLSintax() = 0;
   ConverterSintaX();
   ~ConverterSintaX();
   void getInputLine(QString p_linefile);
   int test(QString p_linefile);
private:
   int _LineNumber;
   QVector<QString> _InputLine;
   LsDyna::KeywordDyna _mode;
};

#endif // CONVERTERSINTAX_H


/*
void HeaderApdl()
{
    QString header = "FINISH\n";
    header += "/CLEAR,START,NEW\n\n";
    header += "/PREP7";
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
