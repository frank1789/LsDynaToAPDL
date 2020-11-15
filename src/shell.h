#ifndef SHELL_H
#define SHELL_H

#include "finitelement.h"
#include "element.h"
#include "node.h"

/**
 * @brief The Shell class this is specifically designed for shell elements 4
 * nodes present in ANSYS as element SHELL181.
 */
class Shell : public Element
{
public:
    Shell();

    ~Shell() override;

    void readfromfile(const QString &inputline) override;

    long size() override;

    int getElementID(int i);

    int getElementNode(int i, int j);

    double getElementThickness(int i);

    int getElementNumNode();

private:
    propelem<int, int, double, 4> _shelldata; /**< struct of shell to extract information. */

    QVector<propelem<int, int, double, 4>>* _shellimport; /**< vector contains all shell extracted. */

    bool _flagNode;       /**< confirm extraction of node's connection. */

    bool _flagThickness;  /**< confirm extraction shell's thickness. */
};

#endif // SHELL_H
