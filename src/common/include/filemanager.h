#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QString>

using ull_t = unsigned long long;

/**
 * @brief The ManageFile class manages incoming files.
 * @details It allows to extract the information of the incoming files as:
 * name, extension, size, etc.
 * Defines the name of the output file and checks if it already exists
 */
class ManageFile : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Manage File object
   *
   * @param parent
   */
  explicit ManageFile(QObject *parent = nullptr);

  /**
   * @brief Destroy the Manage File object
   *
   */
  ~ManageFile() override;

  void print();

  int getSizelistFile();

  void convert();

  /**
   * @brief Set the Filename object
   *
   * @param filename
   */
  void setFilename(const QString &filename);

  /**
   * @brief Get the Filename object
   *
   * @return QString
   */
  QString getFilename() const;

  QString getOutputfile() const;

 signals:
  void changeOutputFilename(const QString &filename);
  void updatePropertyFile(const QString &label, const qint64 &size);

 public slots:
  void processedFilename(const QString &filename);

 protected:
  void setNewFilename(const QString &filename);
  qint64 extractSizeFile(const QString &filename);

 private:
  QString filename_;
  QString new_filename_;
};

#endif  // FILEMANAGER_H
