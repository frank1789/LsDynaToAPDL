#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QString>

/**
 * @brief The FileManager class manages incoming files.
 * @details It allows to extract informations about files such as:
 * name, extension, size, etc.
 * Furthermore, it generates the name of the output file and checks if it already
 * exists.
 */
class FileManager : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Manage File object.
   *
   * @param parent QObject pointer to parent
   */
  explicit FileManager(QObject *parent = nullptr);

  /**
   * @brief Destroy the Manage File object.
   *
   */
  ~FileManager() override;

  /**
   * @brief Set the Filename object.
   *
   * @param filename stream holds the complete filename.
   */
  void setFilename(const QString &filename);

  void setFilename(const char *filename);

  void setFilename(const std::string &filename);

  /**
   * @brief Get the only filename stream.
   *
   * @return QString
   */
  [[nodiscard]] QString getFilename() const;

  /**
   * @brief Get the complete Filename stream.
   *
   * @return QString
   */
  [[nodiscard]] QString getCompleteFilename() const;

  /**
   * @brief Get the new filename.
   * It holds the original filename within the postfix "_converted.txt"
   *
   * @return QString
   */
  [[nodiscard]] QString getOutputFile() const;

  /**
   * @brief Get the file's dimension in byte.
   *
   * @return qint64
   */
  [[nodiscard]] inline auto getFilesize() const -> qint64 { return file_size_; }

  /**
   * @brief The isValidFile method checks is file exists and ensures is regular
   * file.
   *
   * @param filename stream holds the complete filename.
   * @return true
   * @return false
   */
  [[nodiscard]] static auto isValidFile(const QString &filename) -> bool;

 private:
  void extractFileFeatures(const QString &filename);
  void setNewFilename(const QString &filename);

 signals:
  void outputFilenameChanged(const QString &filename);
  void sizeFileChanged(const qint64 &size);

 private:
  QString complete_filename_{""};
  QString filename_{""};
  QString out_filename_{""};
  qint64 file_size_{0};
};

#endif  // FILEMANAGER_H
