#ifndef SHELLELEMENT_TYPE_IMPL_H
#define SHELLELEMENT_TYPE_IMPL_H

/**
 * @brief The propelem struct defines the general appearance of the parameters.
 * of an element.
 *
 * @details The content of the structure represents the element defined by a
 * unique ID, by the number of nodes that constitute it. The thickness used for
 * shell-like elements.
 * A template formulation is used to make most of the definitions available to
 * the structure.
 * It is suggested to use:
 *  - Integer data types:
 *      - element IDs;
 *      - nodes that constitute it as identified by the ID;
 *      - number of nodes of the element;
 *  - Float Data type:
 *      - thickness.
 *
 * @tparam N: type for elments IDs
 * @tparam EN: type for node IDs
 * @tparam T: type for element's thickness
 * @tparam size: sets the number of nodes of the element
 */
template <typename N, typename EN, typename T, int size>
class ShellElement : public Element<typename N, typename EN> {
 private:
  T thickness_; /**< thickness of the element. */
};

#endif  // SHELLELEMENT_TYPE_IMPL_H