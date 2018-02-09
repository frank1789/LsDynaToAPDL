#include "convertersintax.h"
#include "QDebug"
#include "QVector"
#include "QFile"
#include "QRegularExpression"




ConverterSintaX::ConverterSintaX()
{
  //_LineNumber = pCountLine;
  _InputLine.clear();
  qDebug()<<"Call ConvertSintax()";
  qDebug()<<"set line number: "<<_LineNumber;

}

void ConverterSintaX::setInputLine(QString p_linefile, Node *Node, Shell *Shell)
{
  qDebug()<< p_linefile;
  if(p_linefile.contains("$"))
    {
      qDebug()<<"start reading Header";
      _mode = LsDynaSintax::$;
      qDebug()<<"set mode"<<_mode;
    }

  if(p_linefile.contains("*KEYWORD"))
    {
      qDebug()<<"start reading node declaration";
      _mode = LsDynaSintax::KEYWORD;
      qDebug()<<"set mode"<<_mode;
    }

  if(p_linefile.contains("*NODE"))
    {
      qDebug()<<"start reading node declaration";
      _mode = LsDynaSintax::NODE;
      qDebug()<<"set mode"<<_mode;
    }

  if(p_linefile.contains("*ELEMENT_SHELL_THICKNESS"))
    {
      qDebug()<<"start reading node declaration";
      _mode = LsDynaSintax::ELEMENTSHELL;
      qDebug()<<"set mode"<<_mode;
    }

  if(p_linefile.contains("*ELEMENT_SOLID"))
    {
      qDebug()<<"start reading node declaration";
      _mode = LsDynaSintax::ELEMENTSOLID;
      qDebug()<<"set mode"<<_mode;
    }

  if(p_linefile.contains("*INITIAL_STRAIN_SOLID"))
    {
      qDebug()<<"start reading node declaration";
      _mode = LsDynaSintax::INITIALSTRAINSOLID;
      qDebug()<<"set mode"<<_mode;
    }

  if(p_linefile.contains("*INITIAL_STRESS_SHELL"))
    {
      qDebug()<<"start reading node declaration";
      _mode = LsDynaSintax::INITIALSTRESSSHELL;
      qDebug()<<"set mode"<<_mode;
    }

  ConverterSintaX::test(p_linefile, Node, Shell);

}


ConverterSintaX::~ConverterSintaX()
{
  qDebug()<<"clear the vector of line, and call distructor";
  _InputLine.clear();
}



int ConverterSintaX::test(QString p_linefile, Node *Node, Shell *Shell)
{
  switch (_mode) {
    case LsDynaSintax::$:{
        return -1;
      }break;

    case LsDynaSintax::KEYWORD:{
        return -1;
      }break;

    case LsDynaSintax::NODE:
      {
        Node->readfromfile(p_linefile);
      }
      break;

    case LsDynaSintax::ELEMENTSHELL:
      {
        Shell->readfromfile(p_linefile);
      }
      break;

    case LsDynaSintax::ELEMENTSOLID:
      return -1;
      break;

    case LsDynaSintax::INITIALSTRAINSOLID:
      return -1;
      break;

    case LsDynaSintax::INITIALSTRESSSHELL:
      return -1;
      break;

    default:
      return -1;
      break;
    }
  return 0;
}
