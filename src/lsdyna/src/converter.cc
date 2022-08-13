/**
 * @file converter.h
 * @author Francesco Argentieri (francesco.argentieri89@gmaol.com)
 * @brief The ConverterSyntax interpreter of the LS-Dyna format.
 * @version 0.4.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

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

namespace syntax {
namespace lsdyna {

constexpr quint64 kPresetElements{200000};

ConverterSyntax::ConverterSyntax(QObject *parent) : QThread(parent) {
  nodes_.reserve(kPresetElements);
  elements_.reserve(kPresetElements);
  parser_ = ElementParser::getInstance();
}

/**
 * @brief ConverterSyntax::setInputLine
 *
 * @details The function scrolls the document reading the
 * line in input and checking if it contains one of the
 * specified keywords, if the matching is positive the
 * appropriate mode is saved. Otherwise the lines are
 * transparent to the function and the set mode is not changed.
 *
 * @param[in] textline: line of the document to be analyzed.
 */
void ConverterSyntax::testInputLine(const QString &textline) {
  if (textline.contains("$")) {
    doc_section_ = KeywordDyna::Header;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_;
  }

  if (textline.contains("*KEYWORD")) {
    doc_section_ = KeywordDyna::KeyWord;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_;
  }

  if (textline.contains("*NODE")) {
    doc_section_ = KeywordDyna::Node;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_ << "start reading node declaration";
  }

  if (textline.contains("*ELEMENT_SHELL_THICKNESS")) {
    doc_section_ = KeywordDyna::ElementShell;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_ << "start reading element shell declaration";
  }

  if (textline.contains("*ELEMENT_SOLID")) {
    doc_section_ = KeywordDyna::ElementSolid;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_ << "start reading solid element declaration";
  }

  if (textline.contains("*INITIAL_STRAIN_SOLID")) {
    doc_section_ = KeywordDyna::InitialStrainSolid;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_ << "start reading intial strain solid declaration";
  }

  if (textline.contains("*INITIAL_STRESS_SHELL")) {
    doc_section_ = KeywordDyna::InitialStressShell;
    qDebug().noquote() << INFOFILE << "set mode" << doc_section_ << "start reading initial stress shell declaration";
  }
}

void ConverterSyntax::parseLine(const QString &line) {
  testInputLine(line);
  switch (doc_section_) {
    case KeywordDyna::Header:
      break;

    case KeywordDyna::KeyWord:
      break;

    case KeywordDyna::Node: {
      auto node = Node::parseNode(line);
      nodes_.push_back(node);
    } break;

    case KeywordDyna::ElementShell: {
      parser_->createParser(ShellType::FourNode);
      auto shell_four = parser_->parseElement<ShellFourNode>(line);
      elements_.push_back(shell_four);
    } break;

    case KeywordDyna::ElementSolid:
      break;

    case KeywordDyna::InitialStrainSolid:
      break;

    case KeywordDyna::InitialStressShell:
      break;

    case KeywordDyna::End:
      break;
  }
}

void ConverterSyntax::run() {
  if (filename_.isEmpty()) {
    qWarning().noquote() << INFOFILE
                         << "WARNING - use ConverterSyntax::setInputFile(const "
                            "QString &filename)";
    return;
  }

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
    qWarning().noquote() << "WARNING - Error while opening file:" << file.data()->errorString();
    QMessageBox::information(nullptr, QStringLiteral("Error"), file.data()->errorString());
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

void ConverterSyntax::setInputFile(const QString &filename) {
  if (filename != filename_) {
    filename_ = filename;
  }
}

QVector<PropertyNode<quint64, qreal>> ConverterSyntax::getNodes() const { return nodes_; }

QVector<ShellFourNode> ConverterSyntax::getElements() const { return elements_; }

///////////////////////////////////////////////////////////////////////////////
// Slot
///////////////////////////////////////////////////////////////////////////////

void ConverterSyntax::filenameChanged(const QString &filename) { setInputFile(filename); }

}  // namespace lsdyna
}  // namespace syntax
