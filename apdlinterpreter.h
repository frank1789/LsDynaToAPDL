#ifndef APDLINTERPRETER_H
#define APDLINTERPRETER_H
#include <QString>
#include "lsdynainterpreter.h"

namespace APDLsintax {



class ApdlInterpreter
{
    virtual void setNodeWriter() = 0;
    //virtual void setElementSheelWriter() = 0;
};

class Node : public ApdlInterpreter
{
public:
   Node(QVector<NodeProperty> pvectornode);
   void setNodeWriter();
   QString _header;
   QString _NodeStringOutput;
   QVector<NodeProperty> _NodeToWrite;
};
}

#endif // TYPEELEMENT_H
