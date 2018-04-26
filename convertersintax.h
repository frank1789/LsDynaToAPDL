#ifndef CONVERTERSINTAX_H
#define CONVERTERSINTAX_H
#include "QVector"
#include "QString"
#include "node.h"
#include "shell.h"


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
}

class ConverterSintaX
{
public:
   ConverterSintaX();
   ~ConverterSintaX();
   void setInputLine(QString p_linefile, Node *Node, Shell *Shell);
   int test(QString p_linefile, Node *Node, Shell *Shell);
private:
   int _LineNumber;
   QVector<QString> _InputLine;
   LsDynaSintax::KeywordDyna _mode;
};

#endif // CONVERTERSINTAX_H
