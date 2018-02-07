#include "finitelement.h"
#include "QDebug"

template<>
struct propnode<int, double>
{
  int idnode;
  double coordinate_x;
  double coordinate_y;
  double coordinate_z;
};

template<>
struct propelem<int, int, double, 4>
{
  int idelem;          /**< */
  int nodeelem[4];    /**< */
  double  elemthick;   /**< */
};


FiniteElement::~FiniteElement() { qDebug() << "call ~FiniteElement()";}
Element::~Element() {qDebug() << "call ~Element()";}
