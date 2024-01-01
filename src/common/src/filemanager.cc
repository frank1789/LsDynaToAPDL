#include "filemanager.h"

#include <spdlog/spdlog.h>

FileManager::FileManager(std::string_view filename) noexcept {
  setInputFilename(filename);
}

void FileManager::setInputFilename(const std::filesystem::path& filename) {
  if (isValidFile(filename)) {
    spdlog::info("\"{}\" is valid file", filename.string());
    m_complete_filename = std::filesystem::absolute(filename);
    m_file_size = std::filesystem::file_size(filename);
    m_filename = filename.filename();
  }
}

auto FileManager::getFilename() const -> const std::string& {
  return m_filename;
}

auto FileManager::getCompleteInputFilename() const -> const std::string& {
  return m_complete_filename;
}

auto FileManager::getOutputFilename() const -> const std::string& {
  return m_out_filename;
}

void FileManager::setOutputFilename() {
  //  const QRegularExpression re("(\\.\\w+)");
  //  const QString replace = "_converted.txt";
  //  out_filename_ = filename;
  //  out_filename_.replace(re, replace);

  spdlog::info("output file: \"{}\"", m_out_filename);
}

auto FileManager::getFileSize() const -> std::size_t { return m_file_size; }

bool FileManager::isValidFile(const std::filesystem::path& filename) {
  const bool file_exists = std::filesystem::exists(filename) &&
                           std::filesystem::is_regular_file(filename);
  if (file_exists) {
    return true;
  }

  return false;
}
