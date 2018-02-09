#ifndef NODE_H
#define NODE_H
#include "finitelement.h"

class Node : public FiniteElement
{
public:
  Node();
  ~Node();
  void readfromfile(QString pInputLine);
  long size();
  int getIdNode(int i);
  double getCoordinateX(int i);
  double getCoordinateY(int i);
  double getCoordinateZ(int i);
private:
  propnode<int, double> _datain;
  QVector<propnode<int, double>>* _nodeimport;
};


#endif // NODE_H
