#ifndef LSDYNAINTERPRETER_H
#define LSDYNAINTERPRETER_H
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFile>
#include <QRegularExpression>

struct NodeProperty
{
    QString id;
    QString cordinatex;
    QString cordinatey;
    QString cordinatez;
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
    virtual void setReader(QString pInputFile) = 0;
};

//define class to interpret node
class Node : public LsDynaInterpreter
{
public:
    Node();
    ~Node();
    void setReader(QString pInputFile);
    QVector<NodeProperty> getNodeStructure();
private:
    QRegularExpression re;
    QRegularExpressionMatch _match;
    QString _IDnode;
    QString _CoordinateNodeX;
    QString _CoordinateNodeY;
    QString _CoordinateNodeZ;
    QVector<NodeProperty> _OutputNode;
    NodeProperty _TempOutputNode;
};

//define class to interpret element shell
class ElementShell : public LsDynaInterpreter
{
public:
    ElementShell();
    ~ElementShell();
    void setReader(QString pInputFile);
private:
    int a =0;
};
}

#endif // LSDYNAINTERPRETER_H
