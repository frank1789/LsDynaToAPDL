#include "shell.h"
#include "QDebug"
#include "QRegularExpression"


#define INIT 35000


Shell::Shell()
{
  // initialize flag
  _flagNode = false;
  _flagThickness = false;
//  _nodeelem = new double[N];
  _shellimport = new QVector<propelem<int, int, double, 4>>;
  _shellimport->clear();
  _shellimport->reserve(INIT);
}

Shell::~Shell()
{
  delete _shellimport;
}

void Shell::readfromfile(QString pInputLine)
{
  QRegularExpression _re;
  //  set pattern for search scheme of element definition
  //  extract node element 4 from string -----------------------------------------------------------------+
  //  extract node element 3 from string -------------------------------------------------+               |
  //  extract node element 2 from string --------------------------------+                |               |
  //  extract node element 1 from string ---------------+                |                |               |
  //  extract element card ignored -----+               |                |                |               |
  //  extract id -------+               |               |                |                |               |
  //                    +               +               +                +                +               +
  _re.setPattern("(?<id>\\d+)\\s+(?<card>\\d+)\\s(?<node1>\\d+)\\s(?<node2>\\d+)\\s(?<node3>\\d+)\\s(?<node4>\\d+)");
  // verify captured groups
  QRegularExpressionMatch _match = _re.match(pInputLine);
  if(_match.hasMatch())
    {
      qDebug()<<"has match: "<< _match.hasMatch() <<", fonud groups:" << _re.captureCount();
      _shelldata.idelem = _match.captured("id").toInt(); // capture id element
      _shelldata.nodeelem[0] = _match.captured("node1").toDouble();  // capture node 1
      _shelldata.nodeelem[1] = _match.captured("node2").toDouble();  // capture node 2
      _shelldata.nodeelem[2] = _match.captured("node3").toDouble();  // capture node 3
      _shelldata.nodeelem[3] = _match.captured("node4").toDouble();  // capture node 4
      //      qDebug()<<"form element id:" << shell.IdElement <<", E," << shell.Node_1;
      //      qDebug()<< ","<< shell.Node_2 << "," << shell.Node_3 << "," << shell.Node_4;
      _flagNode = true;
    }
  // verify second line string element thickeness replicated four time costant for element
  _re.setPattern("(\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)");
  _match = _re.match(pInputLine);
  if(_match.hasMatch())
    {
      qDebug()<<"has match: "<<_match.hasMatch() <<", fonud groups:" << _re.captureCount();
      _shelldata.elemthick = _match.captured(1).toDouble();
      qDebug() << "thickness element:" << _shelldata.elemthick;
      _flagThickness = true;
    }
  //  fill vector element
  if(_flagNode == true && _flagThickness == true)
    {
      //      qDebug()<<"inside if id:" << shell.IdElement <<", E," << shell.Node_1 << ",";
      //      qDebug()<< shell.Node_2 << "," << shell.Node_3 << "," << shell.Node_4;
      //      qDebug()<< "thickness element:" << shell.ElemThickness;
            _shellimport->push_back(_shelldata);
      // reset the flag
      _flagNode =false;
      _flagThickness = false;
    }
}

long Shell::size() {return _shellimport->size();}

int Shell::getElementID(int i){return _shellimport->at(i).idelem;}
int Shell::getElementNode(int i, int j){return _shellimport->at(i).nodeelem[j];}
int Shell::getElementNumNode(){return 4;}
double Shell::getElementThickness(int i){return _shellimport->at(i).elemthick;}
