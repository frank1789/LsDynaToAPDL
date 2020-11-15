#ifndef FINITELEMENT_H
#define FINITELEMENT_H

#include <QString>



/**
 * @brief The FiniteElement class to define the type of elements and their structure.
 * @details Provides virtual methods to be defined in the derived classes to
 * better interpret the type of existing finite elements.
 */
class FiniteElement
{
  virtual void readfromfile(const QString &inputline) = 0;
  virtual long size() = 0;

 public:
  virtual ~FiniteElement() = 0;
};















template<typename N, typename EN, typename T, int size>
/**
 * @brief The propelem struct defines the general appearance of the parameters.
 * of an element.
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
 * @tparam N: type for elments IDs
 * @tparam EN: type for node IDs
 * @tparam T: type for element's thickness
 * @tparam size: sets the number of nodes of the element
 */
struct propelem
{
    N idelem;           /**< uniquely identifies element IDs. */
    EN nodeelem[size];  /**< ID of the node that constitutes the element. */
    T  elemthick;       /**< thickness of the element. */
};






#endif // FINITELEMENT_H
