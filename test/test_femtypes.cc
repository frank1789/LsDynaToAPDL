#include <sstream>

#include "finite_element_types.h"
#include "gtest/gtest.h"

TEST(FiniteElementTypes, NodeProperty) {
  auto node = PropertyNode<std::int64_t, double>(375, 33.17, 22.45, 0.0);
  EXPECT_EQ(node.id_node(), 375);
  EXPECT_EQ(node.coordinate_x(), 33.17);
  EXPECT_EQ(node.coordinate_y(), 22.45);
  EXPECT_EQ(node.coordinate_z(), 0.0);
}

TEST(FiniteElementTypes, NodeComparision) {
  EXPECT_TRUE((PropertyNode<u_int64_t, double>(375, 33.17, 22.45, 0.0) ==
               PropertyNode<u_int64_t, double>(375, 33.17, 22.45, 0.0)));
  EXPECT_TRUE((PropertyNode<u_int64_t, double>(375, 33.17, 22.45, 0.0) !=
               PropertyNode<u_int64_t, double>(375, 33.17, 22.45, 1.0)));
  EXPECT_FALSE((PropertyNode<u_int64_t, double>(175498, 33.17, 23.45, 0.0) ==
                PropertyNode<u_int64_t, double>(175498, 33.17, 23.45, 1.00)));
  EXPECT_FALSE((PropertyNode<u_int64_t, double>(175498, 33.17, 23.45, 1.0) !=
                PropertyNode<u_int64_t, double>(175498, 33.17, 23.45, 1.00)));
}

TEST(FiniteElementTypes, ElementProperty) {
  auto shellnode = ShellFourNode();
  const QString example{"1282666       4 1248032 1248085 1248031 1248031\n \
     3.9554682       3.9554682       3.9554682       3.955468"};

  shellnode.parseElement(example);
  // check id
  EXPECT_EQ(shellnode.getId(), 1282666);
  // check nodes
  const QVector<quint64> v1{1248032, 1248085, 1248031, 1248031};
  EXPECT_EQ(shellnode.getNodesId(), v1);

  // check thickness
  EXPECT_EQ(shellnode.getThickness(), 3.9554682);

  const QString another{"1283206       4 1248600 1248624 1248605 1248605\n \
     3.9421899       3.9421899       3.9421899       3.9421899"};

  shellnode.parseElement(another);
  // check id
  EXPECT_EQ(shellnode.getId(), 1283206);
  // check nodes
  const QVector<quint64> v2{1248600, 1248624, 1248605, 1248605};
  EXPECT_EQ(shellnode.getNodesId(), v2);
  // check thickness
  EXPECT_EQ(shellnode.getThickness(), 3.9421899);
}
