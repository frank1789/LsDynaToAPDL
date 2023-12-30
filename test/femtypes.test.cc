#include <fmt/format.h>

#include "finite_element_types.h"
#include "gtest/gtest.h"
#include "node.h"

class LsDynaNodeFixtureTests : public ::testing::Test {
 protected:
  syntax::lsdyna::Node node_parser;
};

auto node = syntax::lsdyna::Node(375, 33.17, 22.45, 0.0);

TEST_F(LsDynaNodeFixtureTests, VerifyNodeParsing) {
  EXPECT_EQ(node_parser.id, 0);
  EXPECT_EQ(node_parser.x, 0.0);
  EXPECT_EQ(node_parser.y, 0.0);
  EXPECT_EQ(node_parser.z, 0.0);
}

TEST_F(LsDynaNodeFixtureTests, VerifyNodeParsingCopyConstructor) {
  node_parser = node;
  // EXPECT_EQ(node, node_parser);
  EXPECT_EQ(node.id, node_parser.id);
  EXPECT_EQ(node.x, node_parser.x);
  EXPECT_EQ(node.y, node_parser.y);
  EXPECT_EQ(node.z, node_parser.z);
}

TEST_F(LsDynaNodeFixtureTests, VerifyNodeParsingMoveConstructor) {
  auto mc_node = std::move(node);
  // EXPECT_EQ(mc_node, node_parser);
  // EXPECT_EQ(syntax::lsdyna::Node(), node);
}

TEST_F(LsDynaNodeFixtureTests, VerifyNodePrint0) {
  const auto node_str = fmt::format("{}", node_parser);
  EXPECT_EQ("0 0.0000000 0.0000000 0.0000000", node_str);
}

class LsDynaNodeMultipleParametersTests
    : public ::testing::TestWithParam<
          std::tuple<std::string_view, uint64_t, double, double, double>> {
 protected:
  syntax::lsdyna::Node node_parser;
};

TEST_P(LsDynaNodeMultipleParametersTests, ParseString) {
  auto [test_value, id, x, y, z] = GetParam();
  node_parser.parse(test_value);
  ASSERT_EQ(id, node_parser.id);
  ASSERT_EQ(x, node_parser.x);
  ASSERT_EQ(y, node_parser.y);
  ASSERT_EQ(z, node_parser.z);
}

TEST_P(LsDynaNodeMultipleParametersTests, ParseStringCtor) {
  auto [test_value, id, x, y, z] = GetParam();
  node_parser = syntax::lsdyna::Node(test_value);
  ASSERT_EQ(id, node_parser.id);
  ASSERT_EQ(x, node_parser.x);
  ASSERT_EQ(y, node_parser.y);
  ASSERT_EQ(z, node_parser.z);
}

INSTANTIATE_TEST_SUITE_P(
    NodeTests, LsDynaNodeMultipleParametersTests,
    ::testing::Values(
        // clang-format off
      std::tuple("1230742     -240.277832      281.437195     -55.9616051     0.0     0.0", 1230742, -240.277832, 281.437195, -55.9616051),
      std::tuple("1234655     -235.380905      275.594604     -55.9579811     0.0     0.0", 1234655, -235.380905, 275.594604, -55.9579811),
      std::tuple("1234656     -230.403625      269.777252     -55.9557076     0.0     0.0", 1234656, -230.403625, 269.777252, -55.9557076),
      std::tuple("1234657     -225.310608      264.010559     -55.9570045     0.0     0.0", 1234657, -225.310608, 264.010559, -55.9570045),
      std::tuple("1234658     -220.083725      258.302856      -55.962204     0.0     0.0", 1234658, -220.083725, 258.302856, -55.962204),
      std::tuple("1234659     -214.703293      252.664764     -55.9718208     0.0     0.0", 1234659, -214.703293, 252.664764, -55.9718208),
      std::tuple("1234660     -209.125793      247.141937     -55.9833794     0.0     0.0", 1234660, -209.125793, 247.141937, -55.9833794),
      std::tuple("1234661     -203.310471       241.73819     -55.9988022     0.0     0.0", 1234661, -203.310471, 241.73819, -55.9988022),
      std::tuple("1234662     -197.182404       236.49971     -56.0206261     0.0     0.0", 1234662, -197.182404, 236.49971, -56.0206261),
      std::tuple("1234663     -190.721664      231.478012     -56.0332222     0.0     0.0", 1234663, -190.721664, 231.478012, -56.0332222),
      std::tuple("1234664     -183.784882      226.769455     -56.0512581     0.0     0.0", 1234664, -183.784882, 226.769455, -56.0512581),
      std::tuple("1234665     -176.271988      222.129166     -56.0907173     0.0     0.0", 1234665, -176.271988, 222.129166, -56.0907173),
      std::tuple("1234666     -168.043533      217.748306     -56.1747513     0.0     0.0", 1234666, -168.043533, 217.748306, -56.1747513),
      std::tuple("1234667     -159.033813      213.209732     -55.8686371     0.0     0.0", 1234667, -159.033813, 213.209732, -55.8686371),
      std::tuple("1234668     -149.447205      209.166138     -54.1208305     0.0     0.0", 1234668, -149.447205, 209.166138, -54.1208305),
      std::tuple("1234669     -139.990265      206.428635      -51.151062     0.0     0.0", 1234669, -139.990265, 206.428635, -51.151062),
      std::tuple("1234670     -130.918671      204.598434     -47.8448601     0.0     0.0", 1234670, -130.918671, 204.598434, -47.8448601),
      std::tuple("1234671     -122.424721      203.881912     -44.4719505     0.0     0.0", 1234671, -122.424721, 203.881912, -44.4719505),
      std::tuple("1234672     -114.244431      203.336288      -41.326828     0.0     0.0", 1234672, -114.244431, 203.336288, -41.326828),
      std::tuple("1234673     -106.203102      202.838013     -38.4156265     0.0     0.0", 1234673, -106.203102, 202.838013, -38.4156265),
      std::tuple("1234674     -98.3009644      202.229294     -35.7500458     0.0     0.0", 1234674, -98.3009644, 202.229294, -35.7500458),
      std::tuple("1234675     -90.5665207      201.833069     -33.2940636     0.0     0.0", 1234675, -90.5665207, 201.833069, -33.2940636),
      std::tuple("1234676     -82.9492569      201.396851     -31.0835438     0.0     0.0", 1234676, -82.9492569, 201.396851, -31.0835438)
    )
    // clang-format on 
);


// TEST(FiniteElementTypes, NodeComparision) {
//   EXPECT_TRUE((PropertyNode<u_int64_t, double>(375, 33.17, 22.45, 0.0) ==
//                PropertyNode<u_int64_t, double>(375, 33.17, 22.45, 0.0)));
//   EXPECT_TRUE((PropertyNode<u_int64_t, double>(375, 33.17, 22.45, 0.0) !=
//                PropertyNode<u_int64_t, double>(375, 33.17, 22.45, 1.0)));
//   EXPECT_FALSE((PropertyNode<u_int64_t, double>(175498, 33.17, 23.45, 0.0) ==
//                 PropertyNode<u_int64_t,
//                 double>(175498, 33.17, 23.45, 1.00)));
//   EXPECT_FALSE((PropertyNode<u_int64_t, double>(175498, 33.17, 23.45, 1.0) !=
//                 PropertyNode<u_int64_t,
//                 double>(175498, 33.17, 23.45, 1.00)));
// }

// TEST(FiniteElementTypes, ElementProperty) {
//   auto shellnode = ShellFourNode();
//   const QString example{
//       "1282666       4 1248032 1248085 1248031 1248031\n \
//      3.9554682       3.9554682       3.9554682       3.955468"};

//   shellnode.parseElement(example);
//   // check id
//   EXPECT_EQ(shellnode.getId(), 1282666);
//   // check nodes
//   const QVector<quint64> v1{1248032, 1248085, 1248031, 1248031};
//   EXPECT_EQ(shellnode.getNodesId(), v1);

//   // check thickness
//   EXPECT_EQ(shellnode.getThickness(), 3.9554682);

//   const QString another{
//       "1283206       4 1248600 1248624 1248605 1248605\n \
//      3.9421899       3.9421899       3.9421899       3.9421899"};

//   shellnode.parseElement(another);
//   // check id
//   EXPECT_EQ(shellnode.getId(), 1283206);
//   // check nodes
//   const QVector<quint64> v2{1248600, 1248624, 1248605, 1248605};
//   EXPECT_EQ(shellnode.getNodesId(), v2);
//   // check thickness
//   EXPECT_EQ(shellnode.getThickness(), 3.9421899);
// }
