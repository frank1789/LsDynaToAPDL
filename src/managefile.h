#ifndef MANAGEFILE_H
#define MANAGEFILE_H

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
  explicit ManageFile(QObject *parent = nullptr);

  ~ManageFile() override;

  void print();

  int getSizelistFile();

  void convert();

 public slots:
  void setSizelist(int size);

  void setFile(const QString &fileName);

 signals:
  void setconvertedname(QString &nameFile);

  void setfileName(const QString &fileName);

  void outputfileName(const QString &fileName);

  void getPropertyFile(const qint64 &dimension, const QString &label);

 private:
  void getfileName();

  void setNewfileName();

  void setPropertyFile();
  QString filename_;
  QString newfilename_;
  QString replace_;
  ull_t counter_;
  unsigned int number_of_file_;
};

#endif  // MANAGEFILE_H
