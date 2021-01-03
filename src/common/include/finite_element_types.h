#ifndef FINITE_ELEMENT_TYPES_H
#define FINITE_ELEMENT_TYPES_H

#include "element_type_impl.h"
#include "node_type_impl.h"

template <typename N, typename P> class PropertyNode;

template <typename N, typename EN, int size> class PropertyElement;

template <typename N, typename EN, typename T, int size>
class ShellElement;

template <class N, class P>
std::ostream &operator<<(std::ostream &os, const PropertyNode<N, P> &node);

#endif // FINITE_ELEMENT_TYPES_H
