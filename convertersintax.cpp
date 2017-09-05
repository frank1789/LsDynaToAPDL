#include "convertersintax.h"


ConverterSintaX::ConverterSintaX()
{
    //_LineNumber = pCountLine;
    _InputLine.clear();
    qDebug()<<"Constructor start...";
    qDebug()<<"set line number: "<<_LineNumber;

}

void ConverterSintaX::setInputLine(QString p_linefile, LsDynaSintax::Node *pNode , LsDynaSintax::ElementShell *pShell)
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

    ConverterSintaX::test(p_linefile, pNode, pShell);

}

ConverterSintaX::~ConverterSintaX()
{
    qDebug()<<"clear the vector of line, and call distructor";
    _InputLine.clear();
}

int ConverterSintaX::test(QString p_linefile, LsDynaSintax::Node *pNode, LsDynaSintax::ElementShell *pShell)
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
        pNode->setReader(p_linefile);
    }
        break;

    case LsDynaSintax::ELEMENTSHELL:
    {
        pShell->setReader(p_linefile);
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
