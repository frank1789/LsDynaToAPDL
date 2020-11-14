#include <string>

#include "gtest/gtest.h"
#include "logger_tools.h"

std::string dummyfunc() { return INFOFILE; };

TEST(Logger, Logger) {
  auto message = dummyfunc();
  EXPECT_EQ(message, "std::string dummyfunc() /Users/francesco/Documents/Project/LsDynaToAPDL/test/test_logger.cc:6] ");
}