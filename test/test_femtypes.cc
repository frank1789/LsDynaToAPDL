#include <sstream>

#include "finite_element_types.h"
#include "gtest/gtest.h"

TEST(FiniteElementTypes, NodeProperty) {
  auto node = PropertyNode<long long, double>(375, 33.17, 22.45, 0.0);
  EXPECT_EQ(node.id_node(), 375);
  EXPECT_EQ(node.coordinate_x(), 33.17);
  EXPECT_EQ(node.coordinate_y(), 22.45);
  EXPECT_EQ(node.coordinate_z(), 0.0);
}

TEST(FiniteElementTypes, NodePrint) {
  auto node = PropertyNode<long long, double>(375, 33.17, 22.45, 0.0);
  std::ostringstream out;
  out << node;
  EXPECT_EQ(out.str(), "[375, 33.170000, 22.450000, 0.000000]");
}

TEST(FiniteElementTypes, ElementProperty) {
  auto shellnode = ShellFourNode();
  const QString example{
      "1282666       4 1248032 1248085 1248031 1248031\n \
     3.9554682       3.9554682       3.9554682       3.955468"};
  const QString another{
      "1283206       4 1248600 1248624 1248605 1248605\n \
     3.9421899       3.9421899       3.9421899       3.9421899"};

  shellnode.parseElement(example);
  shellnode.parseElement(another);
  // check size vector
  EXPECT_EQ(shellnode.getElements<QVector<ShellFourNode>>().size(), 2);

  // check id
  auto elem1 = shellnode.getElements<QVector<ShellFourNode>>()[0];
  auto elem2 = shellnode.getElements<QVector<ShellFourNode>>()[1];
  EXPECT_EQ(elem1.getId(), 1282666);
  EXPECT_EQ(elem2.getId(), 1283206);

  // check nodes
  const QVector<quint64> v1{1248032, 1248085, 1248031, 1248031};
  const QVector<quint64> v2{1248600, 1248624, 1248605, 1248605};
  EXPECT_EQ(elem1.getNodesId(), v1);
  EXPECT_EQ(elem2.getNodesId(), v2);

  // check thickness
  EXPECT_EQ(elem1.getThickness(), 3.9554682);
  EXPECT_EQ(elem2.getThickness(), 3.9421899);
}
