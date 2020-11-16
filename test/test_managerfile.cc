#include "filemanager.h"
#include "gtest/gtest.h"

TEST(Creation, ManageFile) {
  auto manager = ManageFile();
  EXPECT_NO_THROW(ManageFile());
}

TEST(ManageFile, Setup) {
  auto manager = ManageFile();
  manager.processedFilename("path/to/folder/testnofile.k");
  EXPECT_EQ(manager.getFilename(), "testnofile.k");
}

TEST(ManageFile, UpdateName) {
  auto manager = ManageFile();
  manager.processedFilename("path/to/folder/EmptyFile.k");
  EXPECT_EQ(manager.getFilename(), QString("EmptyFile.k"));
  EXPECT_EQ(manager.getOutputfile(),
            QString("path/to/folder/EmptyFile_converted.txt"));
}