/**
 * @file file_manager.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_COMMON_FILE_MANAGER_HH
#define LSDYNA_TO_APDL_COMMON_FILE_MANAGER_HH

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>

#include "fmt/format.h"

/**
 * @brief The FileManager class manages incoming files.
 * @details It allows to extract informations about files such as:
 * name, extension, size, etc.
 * Furthermore, it generates the name of the output file and checks if it
 * already exists.
 */
class FileManager {
 public:
  /**
   * @brief Construct a new File Manager object.
   *
   * @param parent QObject pointer to parent
   */
  explicit FileManager() noexcept = default;

  /**
   * @brief Construct a new File Manager object.
   *
   * @param filename stream holds the complete to the file.
   */
  explicit FileManager(const std::filesystem::path& filename);

  /**
   * @brief Destroy the Manage File object.
   *
   */
  ~FileManager() noexcept = default;

  /**
   * @brief Set the Filename object.
   *
   * @param filename stream holds the complete filename.
   */
  void setInputFilename(const std::filesystem::path& filename);

  void setOutputFilename();

  /**
   * @brief Get the only filename stream.
   *
   * @return QString
   */
  [[nodiscard]] auto getFilename() const -> const std::string&;

  /**
   * @brief Get the complete Filename stream.
   *
   * @return QString
   */
  [[nodiscard]] auto getCompleteInputFilename() const -> const std::string&;

  /**
   * @brief Get the new filename.
   * It holds the original filename within the postfix "_converted.txt"
   *
   * @return QString
   */
  [[nodiscard]] auto getOutputFilename() const -> const std::string&;

  /**
   * @brief Get the File Size expressed in bytes.
   *
   * @return std::size_t
   */
  [[nodiscard]] auto getFileSize() const noexcept -> std::size_t;

  /**
   * @brief The isValidFile method checks is file exists and ensures is regular
   * file.
   *
   * @param filename stream holds the complete filename.
   * @return true
   * @return false
   */
  [[nodiscard]] auto isValidFile(
      const std::filesystem::path& filename) const noexcept -> bool;

  friend std::ostream& operator<<(std::ostream& os, const FileManager& fm);

 private:
  auto reflect() const { return std::tie(m_complete_filename, m_file_size); }

  std::string m_complete_filename{""};
  std::string m_filename{""};
  std::string m_out_filename{""};
  std::size_t m_file_size{0};
};

inline std::ostream& operator<<(std::ostream& os, const FileManager& fm) {
  return os << fmt::format("FileManager: {} {:>d} bytes",
                           fm.getCompleteInputFilename(), fm.getFileSize());
}

#endif  // LSDYNA_TO_APDL_COMMON_FILE_MANAGER_HH
