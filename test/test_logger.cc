#include <string>

#include "logger_tools.h"
#include "gtest/gtest.h"

std::string dummyfunc() { return INFOFILE.toStdString(); };

TEST(Logger, Logger) {
  auto message = dummyfunc();
  EXPECT_EQ(message, "std::string dummyfunc() "
                     "/Users/francesco/Documents/Project/LsDynaToAPDL/test/"
                     "test_logger.cc:6] ");
}