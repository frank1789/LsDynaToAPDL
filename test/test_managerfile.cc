#include "filemanager.h"
#include "gtest/gtest.h"

static constexpr char kExampleFile[] = "../bin/example.k";

TEST(Creation, FileManager) { EXPECT_NO_THROW(FileManager()); }

auto s_manager = FileManager();

TEST(FileManager, ExistFile) {
  EXPECT_TRUE(FileManager::isValidFile(kExampleFile));
}

TEST(FileManager, setNewFilename) {
  s_manager.setFilename(kExampleFile);
  EXPECT_EQ(s_manager.getFilename(), QStringLiteral("example.k"));
  // EXPECT_EQ(s_manager.getCompleteFilename(), QString(kExampleFile));
}

TEST(FileManager, fileSize) { EXPECT_EQ(s_manager.getFilesize(), 5016); }

TEST(FileManager, UpdateName) {
  EXPECT_EQ(s_manager.getOutputfile(),
            QStringLiteral("../bin/example_converted.txt"));
}
