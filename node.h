#ifndef NODE_H
#define NODE_H
#include "finitelement.h"

class Node : public FiniteElement
{
public:
  Node();
  ~Node();
  void readfromfile(QString pInputLine);
  int getIdNode();
  double getCoordinateX();
  double getCoordinateY();
  double getCoordinateZ();
private:
  propnode<int, double> _datain;
  QVector<propnode<int, double>>* _nodeimport;
};


#endif // NODE_H
