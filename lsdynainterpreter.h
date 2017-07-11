#ifndef LSDYNAINTERPRETER_H
#define LSDYNAINTERPRETER_H
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFile>
#include <QRegularExpression>

struct NodeProperty
{
    int id;
    double cordinatex;
    double cordinatey;
    double cordinatez;
};

namespace LsDynaSintax {

class LsDynaInterpreter
{
    virtual void setNodeReader(QString pInputFile) = 0;
};

class Node : public LsDynaInterpreter
{
public:
    Node();
    ~Node();
    void setNodeReader(QString pInputFile);
    QVector<NodeProperty> getNodeStructure();
private:
    QRegularExpression re;
    QRegularExpressionMatch _match;
    QString _IDnode;
    QString _CoordinateNodeX;
    QString _CoordinateNodeY;
    QString _CoordinateNodeZ;
    NodeProperty _TempOutputNode;
    QVector<NodeProperty> _OutputNode;
};

class Element : public LsDynaInterpreter
{
public:

private:
};

}

#endif // LSDYNAINTERPRETER_H
