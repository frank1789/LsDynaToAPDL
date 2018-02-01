#include "lsdynainterpreter.h"
#include <QDebug>

using namespace LsDynaSintax;


/**
 * @brief Shell::Shell
 */
Shell::Shell()
{
  qDebug() << "Call Shell() class";
  _shell4node = new QVector<ShellProperty4node>; // initialize element vector and clear
  _shell4node->clear();
  //initialize flag
  _flagNode =false;
  _flagThickness = false;
}


/**
 * @brief Shell::readfromfile
 * @param pInputLine
 */
void Shell::readfromfile(QString pInputLine)
{
  ShellProperty4node shell;
  //set pattern for search scheme of element definition
  //extract node element 4 from string -----------------------------------------------------------------+
  //extract node element 3 from string -------------------------------------------------+               |
  //extract node element 2 from string --------------------------------+                |               |
  //extract node element 1 from string ---------------+                |                |               |
  //extract element card ignored -----+               |                |                |               |
  //extract id -------+               |               |                |                |               |
  //                  +               +               +                +                +               +
  _re.setPattern("(?<id>\\d+)\\s+(?<card>\\d+)\\s(?<node1>\\d+)\\s(?<node2>\\d+)\\s(?<node3>\\d+)\\s(?<node4>\\d+)");

  //verify captured groups
  _match = _re.match(pInputLine);
  if(_match.hasMatch())
    {
      qDebug()<<"has match: "<< _match.hasMatch() <<", fonud groups:" << _re.captureCount();
      shell.IdElement = _match.captured("id"); //capture id element
      shell.Node_1 = _match.captured("node1");   //capture node 1
      shell.Node_2 = _match.captured("node2");   //capture node 2
      shell.Node_3 = _match.captured("node3");   //capture node 3
      shell.Node_4 = _match.captured("node4");   //capture node 4
      qDebug()<<"form element id:" << shell.IdElement <<", E," << shell.Node_1;
      qDebug()<< ","<< shell.Node_2 << "," << shell.Node_3 << "," << shell.Node_4;
      _flagNode=true;
    }

  //verify second line string element thickeness replicated four time costant for element
  _re.setPattern("(\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)");
  _match = _re.match(pInputLine);
  if(_match.hasMatch())
    {
      qDebug()<<"has match: "<<_match.hasMatch() <<", fonud groups:" << _re.captureCount();
      shell.ElemThickness = _match.captured(1);
      qDebug() << "thickness element:" << shell.ElemThickness;
      _flagThickness = true;
    }

  //fill vector element
  if(_flagNode == true && _flagThickness == true)
    {
      qDebug()<<"inside if id:" << shell.IdElement <<", E," << shell.Node_1 << ",";
      qDebug()<< shell.Node_2 << "," << shell.Node_3 << "," << shell.Node_4;
      qDebug()<< "thickness element:" << shell.ElemThickness;
      _shell4node->append(shell);
      //reset the flag
      _flagNode =false;
      _flagThickness = false;
    }
}


/**
 * @brief Shell::getElement
 * @return
 */
QVector<ShellProperty4node>* Shell::getElement() { return _shell4node;}

/**
 * @brief Shell::reset
 */
void Shell::reset()
{
  // verify the vector is not void then erase
  qDebug() << _shell4node->isEmpty();
  if(!_shell4node->isEmpty())
    {
      _shell4node->clear();
    }
}


/**
 * @brief Shell::~Shell
 */
Shell::~Shell()
{
  qDebug()<<"Call ~Shell()";
  _shell4node->clear();
  delete _shell4node;
}
