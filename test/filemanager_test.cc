#include <gtest/gtest.h>

#include <filesystem>

#include "common/file_manager.hh"

namespace {

/// Directory holding the test fixtures.
auto base_path() -> std::filesystem::path {
#ifdef TEST_FILE_PATH
  return std::filesystem::path{TEST_FILE_PATH};
#else
  return std::filesystem::path{"test"};
#endif
}

class FileMangerFixtureTests : public ::testing::Test {
 protected:
  void SetUp() override {
    m_filemanager.set_input_filename(base_path() / "example.k");
    m_filemanager.set_output_filename();
  }

  FileManager m_filemanager;
};

}  // namespace

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
//   GetParam(); file_manager.set_input_filename(test_value);
//   EXPECT_TRUE(FileManager::is_valid_file(test_value));
//   EXPECT_EQ(file_manager.get_filename(), expected_name);
//   //EXPECT_EQ(file_manager.get_complete_input_filename(), expected_path);
//   EXPECT_EQ(file_manager.get_file_size(), expected_size);
// }

TEST_F(FileMangerFixtureTests, VerifyExistFileStaticMethod) {
  EXPECT_FALSE(
      m_filemanager.is_valid_file(std::filesystem::path("../bin/example2.k")));
}

TEST_F(FileMangerFixtureTests, VerifyExitFileDoesNotExist) {
  EXPECT_EXIT(m_filemanager.set_input_filename("death.txt"),
              testing::ExitedWithCode(2), "");
}

TEST_F(FileMangerFixtureTests, GetTheFileSize) {
  EXPECT_EQ(m_filemanager.get_file_size(), 5016ULL) << m_filemanager;
}

TEST_F(FileMangerFixtureTests, GetOutputFilename) {
  EXPECT_EQ(m_filemanager.get_output_filename(),
            std::string("example_converted.txt"))
      << m_filemanager;
}
