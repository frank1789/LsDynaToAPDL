#include "elementparser.h"

QSharedPointer<ElementParser> ElementParser::p_instance_{nullptr};
QMutex ElementParser::mutex_;

QSharedPointer<ElementParser> ElementParser::getInstance() {
  {
    QMutexLocker lock(&mutex_);
    if (p_instance_ == nullptr) {
      p_instance_ = QSharedPointer<ElementParser>(new ElementParser);
    }
    return p_instance_;
  }
}

void ElementParser::makeParser(ShellType sn, const ElementFactory &factory) {
  switch (sn) {
    case ShellType::FourNode: {
      elem_ = factory.createShellFourNode();

    } break;
  }
}

void ElementParser::parseElement(const QString &l) { elem_->parseElement(l); }

QSharedPointer<PropertyElement> ElementParser::getGenericElement() const {
  return elem_;
}
