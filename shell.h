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

  long size();

  int getElementID(int i);

  int getElementNode(int i, int j);

  double getElementThickness(int i);

  int getElementNumNode();

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
