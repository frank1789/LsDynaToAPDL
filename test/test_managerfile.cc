#include "filemanager.h"
#include "gtest/gtest.h"


#include <QString>

TEST(Creation, ManageFile) {
  auto manager = ManageFile();
  EXPECT_NO_THROW(ManageFile());
  
}

TEST(ManageFile, Setup) {
  auto manager = ManageFile();
  manager.processedFilename("testnofile.k");
  EXPECT_EQ(manager.getFilename(), QString("testnofile.k"));
}

TEST(ManageFile, UpdateName){
  auto manager = ManageFile();
  manager.processedFilename("EmptyFile.k");
  EXPECT_EQ(manager.getFilename(), QString("EmptyFile.k"));
  EXPECT_EQ(manager.getOutputfile(), QString("ApdlConverted.txt"));
  EXPECT_EQ(manager.getOutputfile(), QString("EmptyFileApdlConverted.txt"));
}