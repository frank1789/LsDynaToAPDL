#include "filemanager.h"

#include <filesystem>

#include "gtest/gtest.h"

const auto cwd = std::filesystem::path("../LsDynaToAPDL/test/example.k");

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
//   auto [test_value, expected_name, expected_path, expected_size] = GetParam();
//   file_manager.setInputFilename(test_value);
//   EXPECT_TRUE(FileManager::isValidFile(test_value));
//   EXPECT_EQ(file_manager.getFilename(), expected_name);
//   //EXPECT_EQ(file_manager.getCompleteInputFilename(), expected_path);
//   EXPECT_EQ(file_manager.getFileSize(), expected_size);
// }

TEST(FileManager, VerifyIsNoThrow) { EXPECT_NO_THROW(FileManager()); }

TEST(FileManager, VerifyExistFile) {
  EXPECT_FALSE(
      FileManager::isValidFile(std::filesystem::path("../bin/example2.k")));
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

// TEST(FileManager, fileSize) { EXPECT_EQ(s_manager.getFilesize(), 5016); }

// TEST(FileManager, UpdateName) {
//   EXPECT_EQ(s_manager.getOutputFile(),
//             QStringLiteral("../bin/example_converted.txt"));
// }
