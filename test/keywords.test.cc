#include "keywords.h"

#include <fmt/core.h>

#include "gtest/gtest.h"

class LsDynaKeywordMultipleParametersTests
    : public ::testing::TestWithParam<
          std::tuple<std::string, syntax::lsdyna::KeywordDyna>> {
 protected:
  // LeapYearCalendar leapYearCalendar;
};

TEST_P(LsDynaKeywordMultipleParametersTests, ChecksStringValueFromEnum) {
  auto [expected, value] = GetParam();
  const auto test_val = fmt::format("{}", value);
  ASSERT_EQ(expected, test_val);
}

INSTANTIATE_TEST_SUITE_P(
    KeywordsTests, LsDynaKeywordMultipleParametersTests,
    ::testing::Values(
        std::make_tuple("$", syntax::lsdyna::KeywordDyna::Header),
        std::make_tuple("KEYWORD", syntax::lsdyna::KeywordDyna::KeyWord),
        std::make_tuple("NODE", syntax::lsdyna::KeywordDyna::Node),
        std::make_tuple("ELEMENTSHELL", syntax::lsdyna::KeywordDyna::ElementShell),
        std::make_tuple("ELEMENTSOLID", syntax::lsdyna::KeywordDyna::ElementSolid),
        std::make_tuple("INITIALSTRAINSOLID", syntax::lsdyna::KeywordDyna::InitialStrainSolid),
        std::make_tuple("INITIALSTRESSSHELL", syntax::lsdyna::KeywordDyna::InitialStressShell),
        std::make_tuple("END", syntax::lsdyna::KeywordDyna::End)));
