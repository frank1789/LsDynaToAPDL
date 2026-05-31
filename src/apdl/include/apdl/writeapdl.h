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

#ifndef LSDYNA_TO_APDL_WRITE_APDL_H_
#define LSDYNA_TO_APDL_WRITE_APDL_H_

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace lsdynatoapdl {
namespace apdl {

static constexpr const char* logger_name{"writer-apdl"};

std::shared_ptr<spdlog::logger> setup_logger(
    std::vector<spdlog::sink_ptr> sinks);

// class Writer : public QThread {
//   Q_OBJECT
//  public:
//   /**
//    * @brief Construct a new Writer object
//    *
//    * @param parent QObject parent pointer
//    */
//   explicit Writer(QObject *parent = nullptr);

//   /**
//    * @brief Construct a new Writer object
//    *
//    * @param filename QString holds output filename
//    * @param parent QObject parent pointer
//    */
//   explicit Writer(const QString &filename, QObject *parent = nullptr);

//   /**
//    * @brief Elaborate file.
//    *
//    */
//   void run() override;

//   /**
//    * @brief Set the Filename output.
//    *
//    * @param filename QString hold the entire path to output file.
//    */
//   void setFilename(const QString &filename);

//  private:
//   void writeNode();
//   void writeElement();

//   QString filename_;
//   QScopedPointer<QFile> ptr_file_{nullptr};
// };

class Writer {
 public:
  // void writeNode(const Node& node);
  // void writeElement(const Element& element);
  // void writeKeypoint(const Keypoint& keypoint);

  void set_filename(const std::string& filename) {
    m_filename = filename;
    spdlog::debug("set output filename: {}", m_filename);
  }

  void write_to_file(const std::string& content) {
    if (m_filename.empty()) {
      spdlog::error("Filename is not set, cannot write to file");
      return;
    }
    std::ofstream ofs(m_filename, std::ios::out | std::ios::trunc);
    if (!ofs) {
      spdlog::error("Failed to open file: {}", m_filename);
      return;
    }
    ofs << content << '\n';
    ofs.close();
    spdlog::info("Written to file: {}", m_filename);
  }

 private:
  std::string m_filename;
  // std::shared_ptr<spdlog::logger> m_logger{nullptr};
};

}  // namespace apdl
}  // namespace lsdynatoapdl

#endif  // LSDYNA_TO_APDL_WRITE_APDL_H_
