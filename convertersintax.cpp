#include "convertersintax.h"
#include "QDebug"
#include "QVector"
#include "QFile"
#include "QRegularExpression"




ConverterSintax::ConverterSintax()
{
    qDebug() << "Call ConvertSintax()";
}

void ConverterSintax::setInputLine(QString linefile, Node *Node, Shell *Shell)
{
    qDebug() <<  linefile;
    if(linefile.contains("$"))
    {
        qDebug() << "start reading Header";
        _mode = LsDynaSintax::$;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*KEYWORD"))
    {
        qDebug() << "start reading other keyword";
        _mode = LsDynaSintax::KEYWORD;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*NODE"))
    {
        qDebug() << "start reading node declaration";
        _mode = LsDynaSintax::NODE;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*ELEMENT_SHELL_THICKNESS"))
    {
        qDebug() << "start reading element shell declaration";
        _mode = LsDynaSintax::ELEMENTSHELL;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*ELEMENT_SOLID"))
    {
        qDebug() << "start reading solid element declaration";
        _mode = LsDynaSintax::ELEMENTSOLID;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*INITIAL_STRAIN_SOLID"))
    {
        qDebug() << "start reading intial strain solid declaration";
        _mode = LsDynaSintax::INITIALSTRAINSOLID;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*INITIAL_STRESS_SHELL"))
    {
        qDebug() << "start reading initial stress shell declaration";
        _mode = LsDynaSintax::INITIALSTRESSSHELL;
        qDebug() << "set mode" << _mode;
    }

    ConverterSintax::test(linefile, Node, Shell);
}






int ConverterSintax::test(QString linefile, Node *Node, Shell *Shell)
{
    switch (_mode) {
    case LsDynaSintax::$:
        return -1;
        break;

    case LsDynaSintax::KEYWORD:
        return -1;
        break;

    case LsDynaSintax::NODE:
        Node->readfromfile(linefile);
        break;

    case LsDynaSintax::ELEMENTSHELL:
        Shell->readfromfile(linefile);
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

ConverterSintax::~ConverterSintax()
{
    qDebug() << "clear the vector of line, and call distructor";
}
