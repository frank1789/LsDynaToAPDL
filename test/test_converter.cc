#include "converter.h"
#include "gtest/gtest.h"


#include <QStringList>

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

TEST(ConverterSyntax, Node) {

  // clang-format off
  const auto text_lines = {
    {"*KEYWORD"}
    {"*NODE"}
    {"1230742     -240.277832      281.437195     -55.9616051     0.0     0.0"},
    {"1234655     -235.380905      275.594604     -55.9579811     0.0     0.0"},
    {"1234656     -230.403625      269.777252     -55.9557076     0.0     0.0"},
    {"1234657     -225.310608      264.010559     -55.9570045     0.0     0.0"},
    {"1234658     -220.083725      258.302856      -55.962204     0.0     0.0"},
    {"1234659     -214.703293      252.664764     -55.9718208     0.0     0.0"},
    {"1234660     -209.125793      247.141937     -55.9833794     0.0     0.0"},
    {"1234661     -203.310471       241.73819     -55.9988022     0.0     0.0"},
    {"1234662     -197.182404       236.49971     -56.0206261     0.0     0.0"},
    {"1234663     -190.721664      231.478012     -56.0332222     0.0     0.0"},
    {"1234664     -183.784882      226.769455     -56.0512581     0.0     0.0"},
    {"1234665     -176.271988      222.129166     -56.0907173     0.0     0.0"},
    {"1234666     -168.043533      217.748306     -56.1747513     0.0     0.0"},
    {"1234667     -159.033813      213.209732     -55.8686371     0.0     0.0"},
    {"1234668     -149.447205      209.166138     -54.1208305     0.0     0.0"},
    {"1234669     -139.990265      206.428635      -51.151062     0.0     0.0"},
    {"1234670     -130.918671      204.598434     -47.8448601     0.0     0.0"},
    {"1234671     -122.424721      203.881912     -44.4719505     0.0     0.0"},
    {"1234672     -114.244431      203.336288      -41.326828     0.0     0.0"},
    {"1234673     -106.203102      202.838013     -38.4156265     0.0     0.0"},
    {"1234674     -98.3009644      202.229294     -35.7500458     0.0     0.0"},
    {"1234675     -90.5665207      201.833069     -33.2940636     0.0     0.0"},
    {"1234676     -82.9492569      201.396851     -31.0835438     0.0     0.0"}
  }
  // clang-format on
auto converter = sintax::lsdyna::ConverterSintax();
for(const auto &line: text_lines){
  converter.testInputLine("$");
}


















}