#ifndef ELEMENT_H
#define ELEMENT_H

#include "finite_element_types.h"

/**
 * @brief The Element class extrapolates elements from the file and manage
 * information.
 *
 */
class Element {
public:
  template <class T> T parseElement(const QString &inputline);
};

#endif // ELEMENT_H
