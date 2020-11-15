#include "converter.h"

#include <QDebug>

#include "logger_tools.h"

sintax::lsdyna::ConverterSintax::ConverterSintax(QObject *parent) : QThread(parent) {}

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
  qDebug() << INFOFILE  << textline;
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
    qDebug() << INFOFILE << "set mode" << doc_section_ << "start reading node declaration";
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
      return;
      break;

    case sintax::lsdyna::KeywordDyna::KEYWORD:
      return;
      break;

    case sintax::lsdyna::KeywordDyna::NODE:
    //   node->readfromfile(textline);
      break;

    case sintax::lsdyna::KeywordDyna::ELEMENTSHELL:
    //   shell->readfromfile(textline);
      break;

    case sintax::lsdyna::KeywordDyna::ELEMENTSOLID:
      return;
      break;

    case sintax::lsdyna::KeywordDyna::INITIALSTRAINSOLID:
      return;
      break;

    case sintax::lsdyna::KeywordDyna::INITIALSTRESSSHELL:
      return;
      break;

    default:
      return;
      break;
  }
}
