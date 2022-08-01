#include "converter.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QScopedPointer>
#include <QTextStream>

#include "elementfactory.h"
#include "elementproperty.h"
#include "logger_tools.h"
#include "node.h"

constexpr quint64 kPresetElements{200000};

sintax::lsdyna::ConverterSintax::ConverterSintax(QObject *parent)
    : QThread(parent) {
  nodes_.reserve(kPresetElements);
  elements_.reserve(kPresetElements);
  parser_ = ElementParser::getInstance();
}

/**
 * @brief ConverterSintax::setInputLine
 *
 * @details The function scrolls the document reading the
 * line in input and checking if it contains one of the
 * specified keywords, if the matching is positive the
 * appropriate mode is saved. Otherwise the lines are
 * transparent to the function and the set mode is not changed.
 *
 * @param[in] textline: line of the document to be analyzed.
 */
void sintax::lsdyna::ConverterSintax::testInputLine(const QString &textline) {
  if (textline.contains("$")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::Header;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_;
  }

  if (textline.contains("*KEYWORD")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::KeyWord;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_;
  }

  if (textline.contains("*NODE")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::Node;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_
                       << "start reading node declaration";
  }

  if (textline.contains("*ELEMENT_SHELL_THICKNESS")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::ElementShell;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_
                       << "start reading element shell declaration";
  }

  if (textline.contains("*ELEMENT_SOLID")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::ElementSolid;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_
                       << "start reading solid element declaration";
  }

  if (textline.contains("*INITIAL_STRAIN_SOLID")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::InitialStrainSolid;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_
                       << "start reading intial strain solid declaration";
  }

  if (textline.contains("*INITIAL_STRESS_SHELL")) {
    doc_section_ = sintax::lsdyna::KeywordDyna::InitialStressShell;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_
                       << "start reading initial stress shell declaration";
  }
}

/**
 * @brief ConverterSintax::test select the read mode by invoking the input Node
 * and Shell classes to interpret the data.
 *
 * @param[in] line: line of the document to be analyzed.
 *
 */
void sintax::lsdyna::ConverterSintax::parseLine(const QString &line) {
  testInputLine(line);
  switch (doc_section_) {
    case sintax::lsdyna::KeywordDyna::Header:
      break;

    case sintax::lsdyna::KeywordDyna::KeyWord:
      break;

    case sintax::lsdyna::KeywordDyna::Node: {
      auto node = Node::parseNode(line);
      nodes_.push_back(node);
    } break;

    case sintax::lsdyna::KeywordDyna::ElementShell: {
      parser_->createParser(ShellType::FourNode);
      auto shell_four = parser_->parseElement<ShellFourNode>(line);
      elements_.push_back(shell_four);
    } break;

    case sintax::lsdyna::KeywordDyna::ElementSolid:
      break;

    case sintax::lsdyna::KeywordDyna::InitialStrainSolid:
      break;

    case sintax::lsdyna::KeywordDyna::InitialStressShell:
      break;

    case sintax::lsdyna::KeywordDyna::End:
      break;
  }
}

void sintax::lsdyna::ConverterSintax::run() {
  QMutex mutex;
  QMutexLocker lock(&mutex);
  // clang-format off
  qDebug().noquote() << INFOFILE
           << "acquires thread" 
           << QThread::currentThreadId()
           << "then open file" << filename_;
  // clang-format on
  // read file
  QScopedPointer<QFile> file(new QFile(filename_));
  if (!file->open(QIODevice::ReadOnly)) {
    qWarning().noquote() << "WARNING - Error while opening file:"
                         << file.data()->errorString();
    QMessageBox::information(nullptr, QStringLiteral("Error"),
                             file.data()->errorString());
    return;
  }

  QTextStream in(file.data());
  quint64 counter = 0;
  while (!in.atEnd()) {
    QString textline = in.readLine();
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

QVector<PropertyNode<quint64, qreal>>
sintax::lsdyna::ConverterSintax::getNodes() const {
  return nodes_;
}

QVector<ShellFourNode> sintax::lsdyna::ConverterSintax::getElements() const {
  return elements_;
}

///////////////////////////////////////////////////////////////////////////////
// Slot
///////////////////////////////////////////////////////////////////////////////

void sintax::lsdyna::ConverterSintax::filenameChanged(const QString &filename) {
  if (filename != filename_) {
    this->setInputFile(filename);
  }
}
