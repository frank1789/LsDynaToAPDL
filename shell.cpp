#include "shell.h"
#include "QDebug"
#include "QRegularExpression"


#define INIT 35000

/**
 * @brief Shell::Shell default constructor
 */
Shell::Shell()
{
    // initialize flag
    _flagNode = false;
    _flagThickness = false;
    _shellimport = new QVector<propelem<int, int, double, 4>>;
    _shellimport->clear();
    _shellimport->reserve(INIT);
}

/**
 * @brief Shell::~Shell destructor.
 */
Shell::~Shell()
{
    _shellimport->clear();
    delete _shellimport;
}

/**
 * @brief Shell::readfromfile  implementation to import the information of the
 * shell elements from the source string where it is saved on two lines.
 * @details For this reason, the function extracts information regarding the
 * unique identifier of the element number and the nodes with which it is
 * created, in this specific application there is a 4-node element.
 * If the extraction is successful, the flag on nodes assumes true Boolean value.
 * Thus the information of the shell shell replicate thickness is extracted if
 * constant. A second flag assumes true value, so everything is saved in the
 * struct propelem< N, EN, T, size > and then in the vector.
 * @param[in] pInputLine: line from origin file.
 */
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

/**
 * @brief Shell::size get the size of vector contains list of elements.
 * @return dimension vector's elements.
 */
long Shell::size()
{
    return _shellimport->size();
}

/**
 * @brief Shell::getElementID get the ID's element at specified position
 * of vector.
 * @param[in] i: index of vector element.
 * @return element ID.
 */
int Shell::getElementID(int i)
{
    return _shellimport->at(i).idelem;
}

/**
 * @brief Shell::getElementNode get the i-th node connected at the n-th element
 * @param[in] i: index of the vector element.
 * @param[in] j: index of the array contenet the j-th.
 * @return ID j-th node.
 */
int Shell::getElementNode(int i, int j)
{
    return _shellimport->at(i).nodeelem[j];
}

/**
 * @brief Shell::getElementNumNode get number of node (in this case constant value 4).
 * @return 4 node of shell element SHELL181.
 */
int Shell::getElementNumNode()
{
    return 4;
}

/**
 * @brief Shell::getElementThickness
 * @param[i] i: index of the vector element.
 * @return thickness of n-th element.
 */
double Shell::getElementThickness(int i)
{
    return _shellimport->at(i).elemthick;
}
