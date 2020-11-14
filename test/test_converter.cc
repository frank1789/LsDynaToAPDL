#include "converter.h"
#include "gtest/gtest.h"

TEST(LsDyna, ConverterSyntax) {
  auto converter = sintax::lsdyna::ConverterSintax();
  EXPECT_NO_THROW(converter.testInputLine("$"));
  EXPECT_NO_THROW(converter.testInputLine("*KEYWORD"));
  EXPECT_NO_THROW(converter.testInputLine("*NODE"));
  EXPECT_NO_THROW(converter.testInputLine("*ELEMENT_SHELL_THICKNESS"));
}

// TEST(NodePrint, FiniteElementTypes) {
//   auto node = PropertyNode<long long, double>(375, 33.17, 22.45, 0.0);
//   std::ostringstream out;
//   out << node;
//   EXPECT_EQ(out.str(), "[375, 33.170000, 22.450000, 0.000000]");
// }