#ifndef APDLINTERPRETER_H
#define APDLINTERPRETER_H
#include <QString>
#include "lsdynainterpreter.h"

namespace APDLsintax {

class ApdlInterpreter
{
    virtual void setWriter() = 0;
public:
    virtual ~ApdlInterpreter() = 0;
};

class Node : public ApdlInterpreter
{
public:
   Node(QVector<NodeProperty> pvectornode);
   ~Node();
   void setWriter();
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
};
}

#endif // TYPEELEMENT_H
