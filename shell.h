#ifndef SHELL_H
#define SHELL_H
#include "finitelement.h"
#include "node.h"


class Shell : public Element
{
public:
  Shell();
  ~Shell();
  void readfromfile(QString pInputLine);
private:
  propelem<int, int, double, 4> _shelldata;
  QVector<propelem<int, int, double, 4>>* _shellimport;
//  int _idelem;          /**< */
//  double* _nodeelem;    /**< */
//  double  _elemthick;   /**< */
  bool _flagNode;       /**< */
  bool _flagThickness;  /**< */
};

#endif // SHELL_H
