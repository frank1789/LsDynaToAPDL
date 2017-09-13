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

struct ShellProperty
{
    QString id_element;
    QString node1;
    QString node2;
    QString node3;
    QString node4;
    QString thickness;
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
public:
    virtual ~LsDynaInterpreter() = 0;
};

//define class to interpret node
class Node : public LsDynaInterpreter
{

public:
    Node();

    ~Node();

    void setReader(QString pInputFile);

    void Clear();

    QVector<NodeProperty> getNodeStructure();

private:
    QRegularExpression _re;

    QRegularExpressionMatch _match;

    QString _IDnode;

    QString _CoordinateNodeX;

    QString _CoordinateNodeY;

    QString _CoordinateNodeZ;

    QVector<NodeProperty> _OutputNode;

    NodeProperty _TempOutputNode;
};

//define class to interpreter element shell
class ElementShell : public LsDynaInterpreter
{

public:
    ElementShell();

    ~ElementShell();

    void setReader(QString pInputFile);

    QVector<ShellProperty> getElementStructure();

    void Clear();

private:
    QRegularExpression _re;

    QRegularExpressionMatch _match;

    ShellProperty _shell4node;

    QVector<ShellProperty> _OutputElmentShell;

    bool _flagNode, _flagThickness;
};
};

#endif // LSDYNAINTERPRETER_H
