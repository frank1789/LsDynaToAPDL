#include "convertersintax.h"
#include <QDebug>


using namespace lsdynasintax;

/**
 * @brief ConverterSintax::ConverterSintax default constructor.
 */
ConverterSintax::ConverterSintax()
{
    qDebug() << "Call ConvertSintax()";
}

/**
 * @brief ConverterSintax::setInputLine
 * @param[in] linefile: line of the document to be analyzed.
 * @param[in] node: pointer to the Node class.
 * @param[in] shell: pointer to the Shell class.
 * @details The function scrolls the document reading the
 * line in input and checking if it contains one of the
 * specified keywords, if the matching is positive the
 * appropriate mode is saved. Otherwise the lines are
 * transparent to the function and the set mode is not changed.
 */
void ConverterSintax::setInputLine(QString linefile, Node *node, Shell *shell)
{
    qDebug() <<  linefile;
    if(linefile.contains("$"))
    {
        qDebug() << "start reading Header";
        _mode = $;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*KEYWORD"))
    {
        qDebug() << "start reading other keyword";
        _mode = KEYWORD;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*NODE"))
    {
        qDebug() << "start reading node declaration";
        _mode = NODE;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*ELEMENT_SHELL_THICKNESS"))
    {
        qDebug() << "start reading element shell declaration";
        _mode = ELEMENTSHELL;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*ELEMENT_SOLID"))
    {
        qDebug() << "start reading solid element declaration";
        _mode = ELEMENTSOLID;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*INITIAL_STRAIN_SOLID"))
    {
        qDebug() << "start reading intial strain solid declaration";
        _mode = INITIALSTRAINSOLID;
        qDebug() << "set mode" << _mode;
    }

    if(linefile.contains("*INITIAL_STRESS_SHELL"))
    {
        qDebug() << "start reading initial stress shell declaration";
        _mode = INITIALSTRESSSHELL;
        qDebug() << "set mode" << _mode;
    }

    ConverterSintax::test(linefile, node, shell);
}

/**
 * @brief ConverterSintax::test select the read mode by invoking the input Node
 * and Shell classes to interpret the data.
 * @param[in] linefile: line of the document to be analyzed.
 * @param[in] node: pointer to the Node class.
 * @param[in] shell: pointer to the Shell class.
 * @return
 */
void ConverterSintax::test(QString linefile,  Node *node, Shell *shell)
{
    switch (_mode) {
    case $:
        return;
        break;

    case KEYWORD:
        return;
        break;

    case NODE:
        node->readfromfile(linefile);
        break;

    case ELEMENTSHELL:
        shell->readfromfile(linefile);
        break;

    case ELEMENTSOLID:
        return;
        break;

    case INITIALSTRAINSOLID:
        return;
        break;

    case INITIALSTRESSSHELL:
        return;
        break;

    default:
        return;
        break;
    }
}

/**
 * @brief ConverterSintax::~ConverterSintax destructor.
 */
ConverterSintax::~ConverterSintax()
{
    qDebug() << "clear the vector of line, and call destructor";
}
