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

//define keyword for languange LsDyna to move different mode to interpret the data
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

//define virtual class for interpreter
class LsDynaInterpreter
{
    virtual void setNodeReader(QString pInputFile) = 0;
};

//define class to interpret node
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

//define class to interpret element shell
class ElementShell : public LsDynaInterpreter
{
public:
    ElementShell();
private:
};

}

#endif // LSDYNAINTERPRETER_H
