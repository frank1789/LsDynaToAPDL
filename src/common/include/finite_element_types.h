#ifndef FINITE_ELEMENT_TYPES_H
#define FINITE_ELEMENT_TYPES_H

#include "element_shell_four_node.h"
#include "node_type_impl.h"

template <class N, class P>
std::ostream &operator<<(std::ostream &os, const PropertyNode<N, P> &node);

enum class ShellType : int {
  FourNode,
};

#endif  // FINITE_ELEMENT_TYPES_H
