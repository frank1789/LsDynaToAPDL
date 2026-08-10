#include "common/file_manager.hh"

#include <spdlog/spdlog.h>

#include <regex>

FileManager::FileManager(const std::filesystem::path& filename) {
  set_input_filename(filename);
}

void FileManager::set_input_filename(const std::filesystem::path& filename) {
  if (!is_valid_file(filename)) {
    spdlog::error("No such file or directory: {}", filename.string());
    std::exit(2);
  }

  spdlog::info("\"{}\" is valid file", filename.string());
  m_complete_filename = std::filesystem::absolute(filename).string();
  m_file_size = std::filesystem::file_size(filename);
  m_filename = filename.filename().string();
}

auto FileManager::get_filename() const -> const std::string& {
  return m_filename;
}

auto FileManager::get_complete_input_filename() const -> const std::string& {
  return m_complete_filename;
}

auto FileManager::get_output_filename() const -> const std::string& {
  return m_out_filename;
}

void FileManager::set_output_filename() {
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

auto FileManager::get_file_size() const noexcept -> std::size_t {
  return m_file_size;
}

bool FileManager::is_valid_file(
    const std::filesystem::path& filename) noexcept {
  return std::filesystem::exists(filename) &&
         std::filesystem::is_regular_file(filename);
}
