#include "lsdyna/keywords.hh"

#include <fmt/core.h>
#include <gtest/gtest.h>

namespace {

class LsDynaKeywordMultipleParametersTests
    : public ::testing::TestWithParam<
          std::tuple<std::string, lsdynatoapdl::lsdyna::KeywordDyna>> {
 protected:
};

}  // namespace

TEST_P(LsDynaKeywordMultipleParametersTests, ChecksStringValueFromEnum) {
  auto [expected, value] = GetParam();
  const auto test_val = fmt::format("{}", value);
  ASSERT_EQ(expected, test_val);
}

INSTANTIATE_TEST_SUITE_P(
    KeywordsTests, LsDynaKeywordMultipleParametersTests,
    ::testing::Values(
        std::make_tuple("$", lsdynatoapdl::lsdyna::KeywordDyna::Header),
        std::make_tuple("KEYWORD", lsdynatoapdl::lsdyna::KeywordDyna::KeyWord),
        std::make_tuple("NODE", lsdynatoapdl::lsdyna::KeywordDyna::Node),
        std::make_tuple("ELEMENTSHELL",
                        lsdynatoapdl::lsdyna::KeywordDyna::ElementShell),
        std::make_tuple("ELEMENTSOLID",
                        lsdynatoapdl::lsdyna::KeywordDyna::ElementSolid),
        std::make_tuple("INITIALSTRAINSOLID",
                        lsdynatoapdl::lsdyna::KeywordDyna::InitialStrainSolid),
        std::make_tuple("INITIALSTRESSSHELL",
                        lsdynatoapdl::lsdyna::KeywordDyna::InitialStressShell),
        std::make_tuple("END", lsdynatoapdl::lsdyna::KeywordDyna::End)));
