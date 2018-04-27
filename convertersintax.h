#ifndef CONVERTERSINTAX_H
#define CONVERTERSINTAX_H
#include <QString>
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

class ConverterSintax
{
public:
   explicit ConverterSintax();
   ~ConverterSintax();
   void setInputLine(QString linefile, Node *Node, Shell *Shell);
   int test(QString p_linefile, Node *Node, Shell *Shell);
private:
   LsDynaSintax::KeywordDyna _mode;
};

#endif // CONVERTERSINTAX_H
