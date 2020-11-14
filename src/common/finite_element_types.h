#ifndef FINITE_ELEMENT_TYPES_H
#define FINITE_ELEMENT_TYPES_H

#include "node_type_impl.h"

template <typename N, typename P>
class PropertyNode;

template <class N, class P>
std::ostream &operator<<(std::ostream &os, const PropertyNode<N, P> &node);

#endif  // FINITE_ELEMENT_TYPES_H
