#ifndef NODE_H
#define NODE_H

#include "finite_element_types.h"

/**
 * @brief The Node class extrapolates the nodes from the file and manage
 * information.
 *
 */
class Node {
 public:
  static PropertyNode<quint64, qreal> parseNode(const QString &inputline);
};

#endif  // NODE_H
