#ifndef LSDYNAINTERPRETER_H
#define LSDYNAINTERPRETER_H
#include "finitelement.h"
#include <QString>
#include <QVector>
#include <QRegularExpression>


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
    virtual void readfromfile(QString pInputLine) = 0;
    virtual void reset() = 0;
  public:
    virtual ~LsDynaInterpreter() = 0;
  };

  //define class to interpret node
  class Node : public LsDynaInterpreter
  {
  public:
    Node();
    ~Node();
    void readfromfile(QString pInputLine);
    void reset();
    QVector<NodeProperty>* getNode();

  private:
    QRegularExpression _re;
    QRegularExpressionMatch _match;
    QVector<NodeProperty>* _node;
  };



  //define class to interpreter element shell
  class Element : public LsDynaInterpreter
  {

  public:
    virtual ~Element();
  };

  //define class to interpreter element shell
  class Shell : public Element
  {
  public:
    Shell();
    ~Shell();
    void readfromfile(QString pInputLine);
    void reset();
    QVector<ShellProperty4node>* getElement();

  private:
    QRegularExpression _re;
    QRegularExpressionMatch _match;
    QVector<ShellProperty4node>* _shell4node;
    bool _flagNode;
    bool _flagThickness;
  };
};

#endif // LSDYNAINTERPRETER_H
