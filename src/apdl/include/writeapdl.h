/**
 * @file writeapdl.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Writer is a class that writes information within APDL syntax.
 * @version 0.4.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef WRITE_APDL_H
#define WRITE_APDL_H

#include <QFile>
#include <QScopedPointer>
#include <QString>
#include <QThread>

namespace apdl {

class Writer : public QThread {
  Q_OBJECT
 public:
  /**
   * @brief Construct a new Writer object
   *
   * @param parent QObject parent pointer
   */
  explicit Writer(QObject *parent = nullptr);

  /**
   * @brief Construct a new Writer object
   *
   * @param filename QString holds output filename
   * @param parent QObject parent pointer
   */
  explicit Writer(const QString &filename, QObject *parent = nullptr);

  /**
   * @brief Elaborate file.
   *
   */
  void run() override;

  /**
   * @brief Set the Filename output.
   *
   * @param filename QString hold the entire path to output file.
   */
  void setFilename(const QString &filename);

 private:
  void writeNode();
  void writeElement();

  QString filename_;
  QScopedPointer<QFile> ptr_file_{nullptr};
};

}  // namespace apdl

#endif  // WRITE_APDL_H
