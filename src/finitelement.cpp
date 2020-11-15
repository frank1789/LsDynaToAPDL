#include "finitelement.h"
#include <QDebug>

//template<>
///**
// * @brief The propnode<int, double> struct specializzation.
// * @see propnode
// */
//struct propnode<int, double>
//{
//  int idnode;
//  double coordinate_x;
//  double coordinate_y;
//  double coordinate_z;
//};

template<>
/**
 * @brief The propelem<int, int, double, 4> struct specializzation
 * element 4-node shell.
 * @see propelem
 * @details This implementation uses 4-node shell elements with thickness, is used
 * to represent the SHELL181 element present in Ansys.
 */
struct propelem<int, int, double, 4>
{
  int idelem;
  int nodeelem[4];
  double elemthick;
};

/**
 * @brief FiniteElement::~FiniteElement default distructor abstract class.
 */
FiniteElement::~FiniteElement() { qDebug() << "call ~FiniteElement()";}
