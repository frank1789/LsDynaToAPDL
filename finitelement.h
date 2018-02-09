#ifndef FINITELEMENT_H
#define FINITELEMENT_H
#include <QString>

template<typename N, typename P>
struct propnode
{
  N idnode; /**< */
  P coordinate_x; /**< */
  P coordinate_y; /**< */
  P coordinate_z; /**< */
};

template<typename N, typename EN, typename T, int size>
struct propelem
{
  N idelem;           /**< */
  EN nodeelem[size];  /**< */
  T  elemthick;       /**< */
};


/**
 * @brief The FiniteElement class
 */
class FiniteElement
{
  virtual void readfromfile(QString pInputLine) = 0;
  virtual long size() = 0;
public:
  virtual ~FiniteElement() = 0;
};


class Element : public FiniteElement
{
public:
  ~Element() = 0;
  void setMaterial();
protected:
  double _moduleYoung;  /**< */
  double _poissonCoeff; /**< */
  int _numofnode; /**< */
};




#endif // FINITELEMENT_H
