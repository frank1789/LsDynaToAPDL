#ifndef APDLINTERPRETER_H
#define APDLINTERPRETER_H
#include <QFile>
#include <QString>
#include <QTextStream>

#include "lsdynainterpreter.h"

namespace APDLsintax {

void Writer(QString pfilename, QVector<NodeProperty> pVectorNode, QVector<ShellProperty> pVectorElementShell);
}
/*
class ApdlInterpreter
{
    virtual void setWriter() = 0;
public:
    virtual ~ApdlInterpreter() = 0;
};

class Node : public ApdlInterpreter
{
public:
   Node(QVector<NodeProperty> pVectorNode);
   ~Node();
   void setWriter();
private:
   QString _header;
   QString _NodeStringOutput;
   QVector<NodeProperty> _NodeToWrite;
};

class ElementShell : public ApdlInterpreter
{
public:
    ElementShell();
    ~ElementShell();
    void setWriter();
private:
    int a;

};
}
*/
#endif  // TYPEELEMENT_H
