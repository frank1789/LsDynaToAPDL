#ifndef LSDYNA_TO_APDL_FILE_MANAGER_H
#define LSDYNA_TO_APDL_FILE_MANAGER_H

#include <filesystem>
#include <string>
#include <string_view>

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
   * @brief Construct a new Manage File object.
   *
   * @param parent QObject pointer to parent
   */
  explicit FileManager() noexcept = default;

  explicit FileManager(std::string_view filename) noexcept;

  /**
   * @brief Destroy the Manage File object.
   *
   */
  ~FileManager() = default;

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
   * @brief Get the file's dimension in byte.
   *
   * @return qint64
   */
  [[nodiscard]] auto getFileSize() const -> std::size_t;

  /**
   * @brief The isValidFile method checks is file exists and ensures is regular
   * file.
   *
   * @param filename stream holds the complete filename.
   * @return true
   * @return false
   */
  [[nodiscard]] static auto isValidFile(const std::filesystem::path& filename)
      -> bool;

 private:
  std::string m_complete_filename{""};
  std::string m_filename{""};
  std::string m_out_filename{""};
  std::size_t m_file_size{0};
};

#endif  // LSDYNA_TO_APDL_FILE_MANAGER_H
