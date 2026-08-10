/**
 * @file converter_test.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Tests for the LS-DYNA deck converter and the *NODE parser.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "lsdyna/converter.hh"

#include <fmt/core.h>
#include <gtest/gtest.h>

#include <clocale>
#include <filesystem>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "apdl/model.hh"
#include "lsdyna/fields.hh"
#include "lsdyna/parser_node.hh"

namespace {

using lsdynatoapdl::apdl::Model;
using lsdynatoapdl::lsdyna::ConverterSyntax;
using lsdynatoapdl::lsdyna::KeywordDyna;
using lsdynatoapdl::lsdyna::ParserNode;

/// Directory holding the test fixtures.
auto base_path() -> std::filesystem::path {
#ifdef TEST_FILE_PATH
  return std::filesystem::path{TEST_FILE_PATH};
#else
  return std::filesystem::path{"test"};
#endif
}

class ConverterSyntaxFixtureTests : public ::testing::Test {
 protected:
  ConverterSyntax m_converter;
};

}  // namespace

// ---------------------------------------------------------------------------
// Field splitting
// ---------------------------------------------------------------------------

TEST(LsDynaFields, SplitsOnRunsOfWhitespace) {
  const auto fields = lsdynatoapdl::lsdyna::split_fields(
      " 1162886     -25.5204048      320.880554     -161.429962     0.0  0.0");

  ASSERT_EQ(fields.size(), 6U);
  EXPECT_EQ(fields[0], "1162886");
  EXPECT_EQ(fields[3], "-161.429962");
}

TEST(LsDynaFields, AlsoAcceptsCommaSeparatedDecks) {
  const auto fields = lsdynatoapdl::lsdyna::split_fields("1,2.0,3.0,4.0");

  ASSERT_EQ(fields.size(), 4U);
  EXPECT_EQ(fields[1], "2.0");
}

TEST(LsDynaFields, RejectsPartiallyNumericFields) {
  EXPECT_FALSE(lsdynatoapdl::lsdyna::to_uint("12ab").has_value());
  EXPECT_FALSE(lsdynatoapdl::lsdyna::to_double("1.0x").has_value());
  EXPECT_EQ(lsdynatoapdl::lsdyna::to_uint("42").value_or(0), 42U);
}

// to_double has two implementations: std::from_chars where the standard
// library provides it for doubles, and a strtod_l fallback for toolchains that
// do not — Apple Clang below the macOS version that made it available. These
// tests fix the behaviour both have to show, so the two cannot drift apart.
TEST(LsDynaFields, ParsesTheNumbersFoundInDecks) {
  using lsdynatoapdl::lsdyna::to_double;

  // value_or keeps each line a single expression; a wrong parse shows up as a
  // mismatch rather than as an exception from value().
  EXPECT_DOUBLE_EQ(to_double("-25.5204048").value_or(0.0), -25.5204048);
  EXPECT_DOUBLE_EQ(to_double("320.880554").value_or(0.0), 320.880554);
  EXPECT_DOUBLE_EQ(to_double("0.0").value_or(1.0), 0.0);
  EXPECT_DOUBLE_EQ(to_double("3.9788582").value_or(0.0), 3.9788582);
  EXPECT_DOUBLE_EQ(to_double("1e3").value_or(0.0), 1000.0);
  EXPECT_DOUBLE_EQ(to_double("-1.5E-3").value_or(0.0), -0.0015);
  EXPECT_DOUBLE_EQ(to_double("5").value_or(0.0), 5.0);
}

TEST(LsDynaFields, AcceptsOnlyPlainDecimalNumbers) {
  using lsdynatoapdl::lsdyna::to_double;

  // The two parsers disagree left to themselves: from_chars in the general
  // format accepts inf and nan, strtod accepts those plus a leading '+',
  // leading whitespace and hex floats. None of them is a coordinate, so a
  // shared guard refuses them before either parser runs.
  EXPECT_FALSE(to_double(" 1.0").has_value());
  EXPECT_FALSE(to_double("+1.0").has_value());
  EXPECT_FALSE(to_double("0x1p3").has_value());
  EXPECT_FALSE(to_double("inf").has_value());
  EXPECT_FALSE(to_double("nan").has_value());
  EXPECT_FALSE(to_double("").has_value());
  EXPECT_FALSE(to_double("-").has_value());
  EXPECT_FALSE(to_double("abc").has_value());
  EXPECT_FALSE(to_double("1.0 ").has_value());
}

TEST(LsDynaFields, DoesNotDependOnTheGlobalLocale) {
  // A European locale renders the decimal separator as a comma; a locale-aware
  // parser would stop at the dot and silently truncate every coordinate.
  const char* const previous = std::setlocale(LC_NUMERIC, nullptr);
  const std::string saved = previous != nullptr ? previous : "C";

  if (std::setlocale(LC_NUMERIC, "de_DE.UTF-8") == nullptr &&
      std::setlocale(LC_NUMERIC, "it_IT.UTF-8") == nullptr) {  // NOLINT
    GTEST_SKIP() << "no comma-decimal locale installed on this machine";
  }

  EXPECT_DOUBLE_EQ(lsdynatoapdl::lsdyna::to_double("-25.5204048").value_or(0.0),
                   -25.5204048);

  static_cast<void>(std::setlocale(LC_NUMERIC, saved.c_str()));
}

// ---------------------------------------------------------------------------
// Node parsing
// ---------------------------------------------------------------------------

namespace {

class ParserNodeParametersTests
    : public ::testing::TestWithParam<
          std::tuple<std::string_view, std::uint64_t, double, double, double>> {
 protected:
  Model m_model;
  ParserNode m_parser{m_model};
};

}  // namespace

TEST_P(ParserNodeParametersTests, ReadsOneNodeCard) {
  const auto [line, id, x, y, z] = GetParam();

  m_parser.parse(line);

  ASSERT_EQ(m_model.nodes().size(), 1U);
  const auto& node = m_model.nodes().front();
  EXPECT_EQ(node.id(), id);
  EXPECT_DOUBLE_EQ(node.x(), x);
  EXPECT_DOUBLE_EQ(node.y(), y);
  EXPECT_DOUBLE_EQ(node.z(), z);
}

INSTANTIATE_TEST_SUITE_P(
    NodeCards, ParserNodeParametersTests,
    ::testing::Values(
        // clang-format off
      std::tuple("1230742     -240.277832      281.437195     -55.9616051     0.0     0.0", 1230742U, -240.277832, 281.437195, -55.9616051),
      std::tuple("1234655     -235.380905      275.594604     -55.9579811     0.0     0.0", 1234655U, -235.380905, 275.594604, -55.9579811),
      std::tuple("1234662     -197.182404       236.49971     -56.0206261     0.0     0.0", 1234662U, -197.182404, 236.49971, -56.0206261),
      std::tuple("1234670     -130.918671      204.598434     -47.8448601     0.0     0.0", 1234670U, -130.918671, 204.598434, -47.8448601),
      std::tuple("1234676     -82.9492569      201.396851     -31.0835438     0.0     0.0", 1234676U, -82.9492569, 201.396851, -31.0835438)
        // clang-format on
        ));

TEST(ParserNode, SkipsKeywordsAndComments) {
  Model model;
  ParserNode parser{model};

  parser.parse("*NODE");
  parser.parse("$ a comment");
  parser.parse("");

  EXPECT_TRUE(model.nodes().empty());
  EXPECT_EQ(parser.rejected(), 0U) << "structural lines are not malformed data";
}

TEST(ParserNode, RejectsATruncatedCard) {
  Model model;
  ParserNode parser{model};

  parser.parse("1230742     -240.277832");

  EXPECT_TRUE(model.nodes().empty());
  EXPECT_EQ(parser.rejected(), 1U);
}

// ---------------------------------------------------------------------------
// Converter
// ---------------------------------------------------------------------------

TEST_F(ConverterSyntaxFixtureTests, CheckReadiness) {
  const auto infile = base_path() / "example.k";
  EXPECT_FALSE(m_converter.is_ready());
  m_converter.set_input_file(infile);
  EXPECT_TRUE(m_converter.is_ready())
      << fmt::format("the expected file is not valid: \"{}\"", infile.string());
}

TEST_F(ConverterSyntaxFixtureTests, RejectsAMissingFile) {
  m_converter.set_input_file(base_path() / "does_not_exist.k");
  EXPECT_FALSE(m_converter.is_ready());
  EXPECT_FALSE(m_converter.parse());
}

TEST_F(ConverterSyntaxFixtureTests, TracksTheActiveSection) {
  m_converter.parse_line("*KEYWORD");
  EXPECT_EQ(m_converter.current_section(), KeywordDyna::KeyWord);

  m_converter.parse_line("*NODE");
  EXPECT_EQ(m_converter.current_section(), KeywordDyna::Node);

  m_converter.parse_line("*ELEMENT_SHELL_THICKNESS");
  EXPECT_EQ(m_converter.current_section(), KeywordDyna::ElementShell);

  m_converter.parse_line("*END");
  EXPECT_EQ(m_converter.current_section(), KeywordDyna::End);
}

TEST_F(ConverterSyntaxFixtureTests, ReadsNodesAndShellsOfADeck) {
  // clang-format off
  const std::vector<std::string_view> deck{
      {"$ HM_OUTPUT_DECK created by a pre-processor"},
      {"*KEYWORD"},
      {"*NODE"},
      {" 1162886     -25.5204048      320.880554     -161.429962     0.0     0.0"},
      {" 1188851     -19.4350681      316.302582     -161.181335     0.0     0.0"},
      {" 1188852     -12.6946602      312.501923     -160.988754     0.0     0.0"},
      {"*ELEMENT_SHELL_THICKNESS"},
      {" 1229174       4 1189339 1162886 1189341 1189340"},
      {"       3.9788582       3.9788582       3.9788582       3.9788582"},
      {" 1229175       4 1189341 1162886 1188851 1189342"},
      {"       3.9804893       3.9804893       3.9804893       3.9804893"},
      {"*END"},
  };
  // clang-format on

  for (const auto& line : deck) {
    m_converter.parse_line(line);
  }

  EXPECT_EQ(m_converter.model().nodes().size(), 3U);
  EXPECT_EQ(m_converter.model().shells().size(), 2U);
  EXPECT_EQ(m_converter.model().nodes().front().id(), 1162886U);
  EXPECT_EQ(m_converter.model().shells().front().id(), 1229174U);
}

TEST_F(ConverterSyntaxFixtureTests, DiscardsSectionsWithoutAnApdlCounterpart) {
  // clang-format off
  const std::vector<std::string_view> deck{
      {"*NODE"},
      {" 1162886     -25.5204048      320.880554     -161.429962     0.0     0.0"},
      {"*INITIAL_STRESS_SHELL"},
      {" 1229174       1       0       1       0"},
      {"       1.0       2.0       3.0       4.0       5.0       6.0"},
      {"*END"},
  };
  // clang-format on

  for (const auto& line : deck) {
    m_converter.parse_line(line);
  }

  // Without clearing the strategy on an unhandled keyword, the stress rows
  // would keep feeding the node parser and invent nodes.
  EXPECT_EQ(m_converter.model().nodes().size(), 1U);
}
