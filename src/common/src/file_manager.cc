#include "common/file_manager.hh"

#include <spdlog/spdlog.h>

#include <regex>

FileManager::FileManager(const std::filesystem::path& filename) {
  setInputFilename(filename);
}

void FileManager::setInputFilename(const std::filesystem::path& filename) {
  if (!isValidFile(filename)) {
    spdlog::error("No such file or directory: {}", filename.string());
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
  auto filename = std::filesystem::path(m_filename);
  if (filename.empty()) {
    spdlog::error("Filename is empty, cannot set output filename");
    return;
  }

  if (filename.has_extension()) {
    m_out_filename = fmt::format("{}_converted.txt", filename.stem().string());
    spdlog::info("output file: \"{}\"", m_out_filename);
    return;
  }
}

auto FileManager::getFileSize() const noexcept -> std::size_t {
  return m_file_size;
}

bool FileManager::isValidFile(
    const std::filesystem::path& filename) const noexcept {
  return std::filesystem::exists(filename) &&
         std::filesystem::is_regular_file(filename);
}
