#ifndef ELEMENT_H
#define ELEMENT_H

#include "finitelement.h"


/**
 * @brief The Element class defines the information common to the type of element.
 * @details The elements ask to specify the characteristics of their type as:
 *  - truss;
 *  - beam;
 *  - shell;
 *  - solid, etc;
 *  - number of nodes;
 *  - material (Elastic modulus, poisson, etc;).
 *
 * All these characteristics are shared between different elements, in fact this
 * abstract calsse allows to share such data with the classes of implementation of
 * the elements.
 */
class Element : public FiniteElement
{
 public:
  ~Element();

  void setMaterial();

 protected:
  qreal young_module_;  /**< Value of the elastic modulus of the material. */
  qreal poisson_coefficient_; /**< Value of the poisson coefficient. */
  int node_number_; /**< Number of nodes cnnected by element type. */
};

#endif // ELEMENT_H
