#include "converter.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QScopedPointer>
#include <QTextStream>

#include "elementfactory.h"
#include "logger_tools.h"
#include "node.h"

constexpr quint64 kPresetElements{200000};

sintax::lsdyna::ConverterSintax::ConverterSintax(QObject *parent)
    : QThread(parent) {
  nodes_.reserve(kPresetElements);
  parser_ = ElementParser::getInstance();
}

/**
 * @brief ConverterSintax::setInputLine
 *
 * @param[in] textline: line of the document to be analyzed.
 * @param[in] node: pointer to the Node class.
 * @param[in] shell: pointer to the Shell class.
 * @details The function scrolls the document reading the
 * line in input and checking if it contains one of the
 * specified keywords, if the matching is positive the
 * appropriate mode is saved. Otherwise the lines are
 * transparent to the function and the set mode is not changed.
 */
void sintax::lsdyna::ConverterSintax::testInputLine(const QString &textline) {
  qDebug() << INFOFILE << textline;
  if (textline.contains("$")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::$;
    qDebug() << INFOFILE << "set mode" << doc_section_;
  }

  if (textline.contains("*KEYWORD")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::KEYWORD;
    qDebug() << INFOFILE << "set mode" << doc_section_;
  }

  if (textline.contains("*NODE")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::NODE;
    qDebug() << INFOFILE << "set mode" << doc_section_
             << "start reading node declaration";
  }

  if (textline.contains("*ELEMENT_SHELL_THICKNESS")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::ELEMENTSHELL;
    qDebug() << INFOFILE << "set mode" << doc_section_
             << "start reading element shell declaration";
  }

  if (textline.contains("*ELEMENT_SOLID")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::ELEMENTSOLID;
    qDebug() << INFOFILE << "set mode" << doc_section_
             << "start reading solid element declaration";
  }

  if (textline.contains("*INITIAL_STRAIN_SOLID")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::INITIALSTRAINSOLID;
    qDebug() << INFOFILE << "set mode" << doc_section_
             << "start reading intial strain solid declaration";
  }

  if (textline.contains("*INITIAL_STRESS_SHELL")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::INITIALSTRESSSHELL;
    qDebug() << INFOFILE << "set mode" << doc_section_
             << "start reading initial stress shell declaration";
  }
}

/**
 * @brief ConverterSintax::test select the read mode by invoking the input Node
 * and Shell classes to interpret the data.
 *
 * @param[in] textline: line of the document to be analyzed.
 * @param[in] node: pointer to the Node class.
 * @param[in] shell: pointer to the Shell class.
 *
 */
void sintax::lsdyna::ConverterSintax::parseLine(const QString &line) {
  switch (doc_section_) {
  case sintax::lsdyna::KeywordDyna::$:
    break;

  case sintax::lsdyna::KeywordDyna::KEYWORD:
    break;

  case sintax::lsdyna::KeywordDyna::NODE: {
    auto node = function_parser_(line);
    nodes_.push_back(node);
  } break;

  case sintax::lsdyna::KeywordDyna::ELEMENTSHELL: {
    QScopedPointer<ShellFactory> shell_parser(new ShellFactory);
    parser_->makeParser(ShellType::FourNode, *shell_parser);
    parser_->parseElement(line);
    //    elements_ = parser_->getElements<QVector<ShellFourNode>>();
  } break;

  case sintax::lsdyna::KeywordDyna::ELEMENTSOLID:
    break;

  case sintax::lsdyna::KeywordDyna::INITIALSTRAINSOLID:
    break;

  case sintax::lsdyna::KeywordDyna::INITIALSTRESSSHELL:
    break;

  default:
    break;
  }
}

void sintax::lsdyna::ConverterSintax::run() {
  QMutex mutex;
  QMutexLocker lock(&mutex);
  // clang-format off
  qDebug() << INFOFILE 
           << "acquires thread" 
           << QThread::currentThreadId()
           << "then open file" << filename_;
  // clang-format on
  // read file
  QScopedPointer<QFile> file(new QFile(filename_));
  if (!file->open(QIODevice::ReadOnly)) {
    QMessageBox::information(nullptr, "error", file.data()->errorString());
  }
  QTextStream in(file.data());
  quint64 counter = 0;
  while (!in.atEnd()) {
    QString textline = in.readLine();
    testInputLine(textline);
    parseLine(textline);
    counter++;
  }
  file->close();
}

void sintax::lsdyna::ConverterSintax::setInputFile(const QString &filename) {
  filename_ = filename;
}

QString sintax::lsdyna::ConverterSintax::getFilename() const {
  return filename_;
}

///////////////////////////////////////////////////////////////////////////////
// Slot
///////////////////////////////////////////////////////////////////////////////

void sintax::lsdyna::ConverterSintax::filenameChanged(
    const QString &filename) {
  if (filename != filename_) {
    this->setInputFile(filename);
  }
}
