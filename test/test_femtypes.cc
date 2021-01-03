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