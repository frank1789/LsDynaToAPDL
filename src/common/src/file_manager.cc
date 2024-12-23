#include "common/file_manager.hh"

#include <regex>

#include "spdlog/spdlog.h"

FileManager::FileManager(const std::filesystem::path& filename) {
  setInputFilename(filename);
}

void FileManager::setInputFilename(const std::filesystem::path& filename) {
  if (!isValidFile(filename)) {
    spdlog::error("[Errno 2] No such file or directory: {}", filename.string());
    std::exit(2);
  }
  spdlog::info("\"{}\" is valid file", filename.string());
  m_complete_filename = std::filesystem::absolute(filename).string();
  m_file_size = std::filesystem::file_size(filename);
  m_filename = filename.filename().string();
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
  const std::regex re("(\\.\\w+)");
  const std::string replace{"_converted.txt"};
  m_out_filename = std::regex_replace(m_filename, re, replace);
  spdlog::info("output file: \"{}\"", m_out_filename);
}

auto FileManager::getFileSize() const noexcept -> std::size_t {
  return m_file_size;
}

bool FileManager::isValidFile(const std::filesystem::path& filename) {
  return std::filesystem::exists(filename) &&
         std::filesystem::is_regular_file(filename);
}
