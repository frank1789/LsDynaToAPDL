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

  void setNewFilename();


QString getOutputfile() const;

 public slots:
  void processedFilename(const QString& filename);





  // void setSizelist(int size);

  // void setFile(const QString &fileName);

 signals:
  void changeOutputFilename(const QString &);
  

  void updatePropertyFile(const QString &label, const qint64 &size);

 private:
  void getfileName();

  void setNewfileName();

  void setPropertyFile();
  QString filename_;
  QString new_filename_;
  ull_t counter_;
  unsigned int number_of_file_;
};

#endif  // FILEMANAGER_H
