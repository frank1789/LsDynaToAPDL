#ifndef NODE_H
#define NODE_H

#include "finitelement.h"
#include "finite_element_types.h"

/**
 * @brief The Node class extrapolates the nodes from the file and manage information.
 * @details
 */
class Node : public FiniteElement
{
public:
    Node();

    ~Node();

    void readfromfile(const QString &inputline) override;

    // long size() override;

    // int getIdNode(int i);

    // double getCoordinateX(int i);

    // double getCoordinateY(int i);

    // double getCoordinateZ(int i);

private:
 PropertyNode<qint64, qreal> data_;
// QVector<PropertyNode<qint64, qreal>> nodeimport_;
};


#endif // NODE_H
