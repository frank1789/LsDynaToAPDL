#include "convertersintax.h"

ConverterSintaX::ConverterSintaX()
{
    _LineNumber = 0;
    _InputLine.clear();
    qDebug()<<"Constructor start...";
    qDebug()<<"set line number: "<<_LineNumber;

}

void ConverterSintaX::getInputLine(QString p_linefile)
{
    qDebug()<< p_linefile;
    if(p_linefile.contains("$"))
    {
        qDebug()<<"start reading Header";
        _mode = LsDyna::$;
        qDebug()<<"set mode"<<_mode;
    }

    if(p_linefile.contains("*KEYWORD"))
    {
        qDebug()<<"start reading node declaration";
        _mode = LsDyna::KEYWORD;
        qDebug()<<"set mode"<<_mode;
    }

    if(p_linefile.contains("*NODE"))
    {
        qDebug()<<"start reading node declaration";
        _mode = LsDyna::NODE;
        qDebug()<<"set mode"<<_mode;
    }

    if(p_linefile.contains("*ELEMENT_SHELL_THICKNESS"))
    {
        qDebug()<<"start reading node declaration";
        _mode = LsDyna::ELEMENTSHELL;
        qDebug()<<"set mode"<<_mode;
    }

    if(p_linefile.contains("*ELEMENT_SOLID"))
    {
        qDebug()<<"start reading node declaration";
        _mode = LsDyna::ELEMENTSOLID;
        qDebug()<<"set mode"<<_mode;
    }

    if(p_linefile.contains("*INITIAL_STRAIN_SOLID"))
    {
        qDebug()<<"start reading node declaration";
        _mode = LsDyna::INITIALSTRAINSOLID;
        qDebug()<<"set mode"<<_mode;
    }

    if(p_linefile.contains("*INITIAL_STRESS_SHELL"))
    {
        qDebug()<<"start reading node declaration";
        _mode = LsDyna::INITIALSTRESSSHELL;
        qDebug()<<"set mode"<<_mode;
    }

    ConverterSintaX::test(p_linefile);

}

ConverterSintaX::~ConverterSintaX()
{
    qDebug()<<"clear the vector of line, and call distructor";
    _InputLine.clear();
}

int ConverterSintaX::test(QString p_linefile)
{
    switch (_mode) {
    case LsDyna::$:{
        return -1;
    }break;

    case LsDyna::KEYWORD:{
        return -1;
    }break;

    case LsDyna::NODE:
    {
        LsDynaSintax::Node* test = new LsDynaSintax::Node();
        test->setNodeReader(p_linefile);
        QVector<NodeProperty> pr = test->getNodeStructure();
        for (int i= 0; i < pr.size(); i++)
            qDebug()<<"print vector id"<<pr[i].id;
        delete test;
    }
    break;

    case LsDyna::ELEMENTSHELL:
        break;

    case LsDyna::ELEMENTSOLID:
        return -1;
        break;

    case LsDyna::INITIALSTRAINSOLID:
        return -1;
        break;

    case LsDyna::INITIALSTRESSSHELL:
        return -1;
        break;

    default:
        return -1;
        break;
    }
}
