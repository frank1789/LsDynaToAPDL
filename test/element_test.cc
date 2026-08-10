/**
 * @file element_test.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Tests for the *ELEMENT_SHELL_THICKNESS m_parser.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

#include "apdl/model.hh"
#include "lsdyna/parser_element.hh"

namespace {

using lsdynatoapdl::apdl::Model;
using lsdynatoapdl::lsdyna::ParserElement;

/// A shell element is spread over two consecutive card lines.
constexpr std::string_view kConnectivity{
    "1282666       4 1248032 1248085 1248031 1248031"};
constexpr std::string_view kThickness{
    "       3.9554682       3.9554682       3.9554682       3.9554682"};

class ParserElementTests : public ::testing::Test {
 protected:
  Model m_model;
  ParserElement m_parser{m_model};
};

}  // namespace

TEST_F(ParserElementTests, ReadsAnElementFromItsTwoLines) {
  m_parser.parse(kConnectivity);
  EXPECT_TRUE(m_parser.awaiting_thickness());
  EXPECT_EQ(m_model.shells().size(), 0U)
      << "the element must not be committed before its thickness is known";

  m_parser.parse(kThickness);
  EXPECT_FALSE(m_parser.awaiting_thickness());
  ASSERT_EQ(m_model.shells().size(), 1U);

  const auto& shell = m_model.shells().front();
  EXPECT_EQ(shell.id(), 1282666U);
  EXPECT_EQ(shell.nodes(), (lsdynatoapdl::apdl::Shell181::NodeIds{
                               1248032, 1248085, 1248031, 1248031}));
  EXPECT_DOUBLE_EQ(shell.thickness(), 3.9554682);
  EXPECT_TRUE(shell.is_degenerate()) << "last two nodes coincide";
}

TEST_F(ParserElementTests, IgnoresTheKeywordLine) {
  m_parser.parse("*ELEMENT_SHELL_THICKNESS");
  EXPECT_FALSE(m_parser.awaiting_thickness());
  EXPECT_EQ(m_parser.accepted(), 0U);
}

TEST_F(ParserElementTests, ReadsAWholeSection) {
  // clang-format off
  const std::vector<std::string_view> lines{
      {"*ELEMENT_SHELL_THICKNESS"},
      {"1269511       4 1234762 1234763 1235160 1235159"},
      {"       4.2463937       4.2463937       4.2463937       4.2463937"},
      {"1269512       4 1235160 1234763 1234764 1235161"},
      {"       4.2409911       4.2409911       4.2409911       4.2409911"},
      {"1269513       4 1235161 1234764 1234765 1235162"},
      {"       4.2597589       4.2597589       4.2597589       4.2597589"},
  };
  // clang-format on

  for (const auto& line : lines) {
    m_parser.parse(line);
  }

  EXPECT_EQ(m_parser.accepted(), 3U);
  EXPECT_EQ(m_parser.rejected(), 0U);
  ASSERT_EQ(m_model.shells().size(), 3U);
  EXPECT_EQ(m_model.shells().back().id(), 1269513U);
  // Three distinct thicknesses means three sections.
  EXPECT_EQ(m_model.sections().size(), 3U);
}

TEST_F(ParserElementTests, AveragesTheFourNodalThicknesses) {
  m_parser.parse("1 4 10 11 12 13");
  m_parser.parse("1.0 2.0 3.0 4.0");

  ASSERT_EQ(m_model.shells().size(), 1U);
  EXPECT_DOUBLE_EQ(m_model.shells().front().thickness(), 2.5);
}

TEST_F(ParserElementTests, RejectsATruncatedConnectivityCard) {
  m_parser.parse("1269511       4 1234762");

  EXPECT_FALSE(m_parser.awaiting_thickness());
  EXPECT_EQ(m_parser.rejected(), 1U);
  EXPECT_TRUE(m_model.shells().empty());
}

TEST_F(ParserElementTests, StaysInSyncAfterABadThicknessCard) {
  m_parser.parse("1 4 10 11 12 13");
  m_parser.parse("not a number here at all");
  // The bad element is dropped, but the next pair must still be read: leaving
  // the state machine waiting would consume the following connectivity card.
  m_parser.parse("2 4 20 21 22 23");
  m_parser.parse("1.0 1.0 1.0 1.0");

  EXPECT_EQ(m_parser.accepted(), 1U);
  ASSERT_EQ(m_model.shells().size(), 1U);
  EXPECT_EQ(m_model.shells().front().id(), 2U);
}
