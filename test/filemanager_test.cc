#include <gtest/gtest.h>

#include <filesystem>

#include "common/file_manager.hh"

#ifdef TEST_FILE_PATH
const auto BasePath = std::filesystem::path{TEST_FILE_PATH};
#else
const auto BasePath = std::filesystem::path{"test"};
#endif

class FileMangerFixtureTests : public ::testing::Test {
 protected:
  void SetUp() override {
    filemanager.setInputFilename(BasePath / "example.k");
    filemanager.setOutputFilename();
  }

  FileManager filemanager;
};

// class FileManagerMultipleParametersTests
//     : public ::testing::TestWithParam<std::tuple<
//           std::filesystem::path, std::string, std::string, std::size_t>> {
//  protected:
//   FileManager file_manager;
// };

// INSTANTIATE_TEST_SUITE_P(
//     InputFileTest, FileManagerMultipleParametersTests,
//     ::testing::Values(
//         std::make_tuple(
//             cwd, "example.k",
//             std::filesystem::absolute(std::filesystem::path("test/example.k")),
//             5016),
//         std::make_tuple(
//             cwd, "example.k",
//             std::filesystem::absolute(std::filesystem::path("test/example.k")),
//             5016),
//         std::make_tuple(
//             cwd, "example.k",
//             std::filesystem::absolute(std::filesystem::path("test/example.k")),
//             5016),
//         std::make_tuple(
//             cwd, "example.k",
//             std::filesystem::absolute(std::filesystem::path("test/example.k")),
//             5016),
//         std::make_tuple(
//             cwd, "example.k",
//             std::filesystem::absolute(std::filesystem::path("example.k")),
//             5016)));

// TEST_P(FileManagerMultipleParametersTests, ChecksInputFiles) {
//   auto [test_value, expected_name, expected_path, expected_size] =
//   GetParam(); file_manager.setInputFilename(test_value);
//   EXPECT_TRUE(FileManager::isValidFile(test_value));
//   EXPECT_EQ(file_manager.getFilename(), expected_name);
//   //EXPECT_EQ(file_manager.getCompleteInputFilename(), expected_path);
//   EXPECT_EQ(file_manager.getFileSize(), expected_size);
// }

TEST_F(FileMangerFixtureTests, VerifyExistFileStaticMethod) {
  EXPECT_FALSE(
      filemanager.isValidFile(std::filesystem::path("../bin/example2.k")));
}

TEST_F(FileMangerFixtureTests, VerifyExitFileDoesNotExist) {
  EXPECT_EXIT(filemanager.setInputFilename("death.txt"),
              testing::ExitedWithCode(2), "");
}

// TEST(FileManager, VerifySetNewFilename) {
//   auto file_manager = FileManager();
//   file_manager.setInputFilename(kExampleFile);
//   EXPECT_EQ(s_manager.getFilename(), "example.k");
//   EXPECT_EQ(s_manager.getCompleteInputFilename(),
//   std::filesystem::current_path() / "example.k");
//   EXPECT_EQ(s_manager.getFilename(), QStringLiteral("example.k"));
//   EXPECT_EQ(s_manager.getFilesize(), 5016);
//   // EXPECT_EQ(s_manager.getCompleteFilename(), QString(kExampleFile));
// }

TEST_F(FileMangerFixtureTests, GetTheFileSize) {
  EXPECT_EQ(filemanager.getFileSize(), 5016ull) << filemanager;
}

TEST_F(FileMangerFixtureTests, GetOutputFilename) {
  EXPECT_EQ(filemanager.getOutputFilename(), std::string("example_converted.txt")) << filemanager;
}
