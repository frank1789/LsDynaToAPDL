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
 * @tparam N: type for node IDs
 * @tparam P: type for spatial coordinate
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


/**
 * @brief The FiniteElement class to define the type of elements and their structure.
 * @details Provides virtual methods to be defined in the derived classes to
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
    ~Element() = 0;

    void setMaterial();

protected:
    double _moduleYoung;  /**< Value of the elastic modulus of the material. */

    double _poissonCoeff; /**< Value of the poisson coefficient. */

    int _numofnode; /**< Number of nodes cnnected by element type. */
};

#endif // FINITELEMENT_H
