#ifndef FINITELEMENT_H
#define FINITELEMENT_H
#include <QString>

/**
 * @brief The NodeStruct struct
 */
struct NodeProperty
{
  QString IdNode;/**< */
  QString coordinateX;/**< */
  QString coordinateY;/**< */
  QString coordinateZ;/**< */
};

/**
 * @brief The shell4node struct
 */
struct ShellProperty4node
{
  QString IdElement;/**< */
  QString Node_1;/**< */
  QString Node_2;/**< */
  QString Node_3;/**< */
  QString Node_4;/**< */
  QString ElemThickness;/**< */
};

#endif // FINITELEMENT_H
