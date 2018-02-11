#ifndef FINITELEMENT_H
#define FINITELEMENT_H
#include <QString>

template<typename N, typename P>
/**
 * @brief The propnode struct contains the information of a single node.
 * @details The information is divided into: the ID of the node that
 * uniquely identifies it, the x coordinate, the y coordinate, the z coordinate.
 * We use a template definition to be able to create multiple definitions
 * depending on the nodes used.
 * It is suggested to use integer data types to define node IDs, while
 * for float data type  coordinates.
 * @param N type for node IDs
 * @param P type for spatial coordinate
 */
struct propnode
{
  N idnode; /**< uniquely identifies node IDs */
  P coordinate_x; /**< x coordinate in the space */
  P coordinate_y; /**< y coordinate in the space */
  P coordinate_z; /**< z coordinate in the space */
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
 * It is suggested to use integer data types:
 * - element IDs;
 * - nodes that constitute it as identified by the ID;
 * - number of nodes of the element;\n
 * Float Data type:
 * - thickness.
 * @param N type for elments IDs
 * @param EN type for node IDs
 * @param T type for element's thickness
 * @param size sets the number of nodes of the element
 */
struct propelem
{
  N idelem;           /**< uniquely identifies element IDs */
  EN nodeelem[size];  /**< ID of the node that constitutes the element */
  T  elemthick;       /**< thickness of the element */
};


/**
 * @brief The FiniteElement class to define the type of elements and their structure.
 * @details provides virtual methods to be defined in the derived classes to
 * better interpret the type of existing finite elements.
 */
class FiniteElement
{
  virtual void readfromfile(QString pInputLine) = 0;
  virtual long size() = 0;
public:
  virtual ~FiniteElement() = 0;
};

/**
 * @brief The Element class defines the information common to the type of element.
 */
class Element : public FiniteElement
{
public:
  ~Element() = 0;
  void setMaterial();
protected:
  double _moduleYoung;  /**< value of the elastic modulus of the material */
  double _poissonCoeff; /**< value of the poisson coefficient */
  int _numofnode; /**< */
};

#endif // FINITELEMENT_H
