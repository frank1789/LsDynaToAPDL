#include "element_shell.h"

#include <QDebug>
#include <QRegularExpression>

#include "logger_tools.h"

Shell::Shell() : node_flag_(false), thickness_flag_(false) {}

/**
 * @brief Implementation to import the information of the
 * shell elements from the source string where it is saved on two lines.
 *
 * @details For this reason, the function extracts information regarding the
 * unique identifier of the element number and the nodes with which it is
 * created, in this specific application there is a 4-node element.
 * If the extraction is successful, the flag on nodes assumes true Boolean
 * value. Thus the information of the shell shell replicate thickness is
 * extracted if constant. A second flag assumes true value, so everything is
 * saved in class
 *
 * @param[in] inputline: line from origin file.
 *
 */
ShellElement<quint64, quint64, qreal, 4>
Shell::parseElement(const QString &inputline) {
  QRegularExpression re;
  // clang-format off
  //  set pattern for search scheme of element definition
  //  extract node element 4 from string -----------------------------------------------------------------+
  //  extract node element 3 from string -------------------------------------------------+               |
  //  extract node element 2 from string --------------------------------+                |               |
  //  extract node element 1 from string ---------------+                |                |               |
  //  extract element card ignored -----+               |                |                |               |
  //  extract id -------+               |               |                |                |               |
  //                    +               +               +                +                +               +
  re.setPattern("(?<id>\\d+)\\s+(?<card>\\d+)\\s(?<node1>\\d+)\\s(?<node2>\\d+)\\s(?<node3>\\d+)\\s(?<node4>\\d+)");
  // clang-format on
  // verify captured groups
  QRegularExpressionMatch match = re.match(inputline);
  if (match.hasMatch()) {
    // clang-format off
    qDebug() << INFOFILE 
            << "has match: " 
            << match.hasMatch()
            << ", fonud groups:" 
            << re.captureCount();
    // clang-format on
    _shelldata.idelem = match.captured("id").toInt(); // capture id element
    _shelldata.nodeelem[0] =
        match.captured("node1").toDouble(); // capture node 1
    _shelldata.nodeelem[1] =
        match.captured("node2").toDouble(); // capture node 2
    _shelldata.nodeelem[2] =
        match.captured("node3").toDouble(); // capture node 3
    _shelldata.nodeelem[3] =
        match.captured("node4").toDouble(); // capture node 4
    //      qDebug()<<"form element id:" << shell.IdElement <<", E," <<
    //      shell.Node_1; qDebug()<< ","<< shell.Node_2 << "," << shell.Node_3
    //      << "," << shell.Node_4;
    node_flag_ = true;
  }
  // verify second line string element thickeness replicated four times costant
  // for element
  // clang-format off
  re.setPattern("(\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)       (\\d+?.\\d+)");
  // clang-format on
  match = re.match(inputline);
  if (match.hasMatch()) {
    // clang-format off
    qDebug() << INFOFILE 
            << "has match: " << match.hasMatch()
            << ", fonud groups:" 
            << re.captureCount();
    // clang-format on
    _shelldata.elemthick = match.captured(1).toDouble();
    qDebug() << INFOFILE << "thickness element:" << _shelldata.elemthick;
    thickness_flag_ = true;
  }
  //  fill vector element
  if (node_flag_ == true && thickness_flag_ == true) {
    //      qDebug()<<"inside if id:" << shell.IdElement <<", E," <<
    //      shell.Node_1 << ","; qDebug()<< shell.Node_2 << "," << shell.Node_3
    //      << "," << shell.Node_4; qDebug()<< "thickness element:" <<
    //      shell.ElemThickness;
    // _shellimport->push_back(_shelldata);
    // reset the flag
    node_flag_ = false;
    thickness_flag_ = false;
  }
}

// /**
//  * @brief Shell::size get the size of vector contains list of elements.
//  * @return dimension vector's elements.
//  */
// long Shell::size()
// {
//     return _shellimport->size();
// }

// /**
//  * @brief Shell::getElementID get the ID's element at specified position
//  * of vector.
//  * @param[in] i: index of vector element.
//  * @return element ID.
//  */
// int Shell::getElementID(int i)
// {
//     return _shellimport->at(i).idelem;
// }

// /**
//  * @brief Shell::getElementNode get the i-th node connected at the n-th
//  element
//  * @param[in] i: index of the vector element.
//  * @param[in] j: index of the array contenet the j-th.
//  * @return ID j-th node.
//  */
// int Shell::getElementNode(int i, int j)
// {
//     return _shellimport->at(i).nodeelem[j];
// }

// /**
//  * @brief Shell::getElementNumNode get number of node (in this case constant
//  value 4).
//  * @return 4 node of shell element SHELL181.
//  */
// int Shell::getElementNumNode()
// {
//     return 4;
// }

// /**
//  * @brief Shell::getElementThickness
//  * @param[i] i: index of the vector element.
//  * @return thickness of n-th element.
//  */
// double Shell::getElementThickness(int i)
// {
//     return _shellimport->at(i).elemthick;
// }