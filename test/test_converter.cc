#include "converter.h"
#include "gtest/gtest.h"

TEST(ConverterSyntax, Keyword) {
  auto converter = sintax::lsdyna::ConverterSintax();
  EXPECT_NO_THROW(converter.testInputLine("$"));
  EXPECT_NO_THROW(converter.testInputLine("*KEYWORD"));
  EXPECT_NO_THROW(converter.testInputLine("*NODE"));
  EXPECT_NO_THROW(converter.testInputLine("*ELEMENT_SHELL_THICKNESS"));
}

TEST(ConverterSyntax, Filename) {
  auto converter = sintax::lsdyna::ConverterSintax();
  converter.changedProcessedFilename("testfile.k");
  EXPECT_EQ(converter.getFilename(), QString("testfile.k"));
}