#include "elementparser.h"

#include <QMutexLocker>

#include "element_shellfournode.h"

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

void ElementParser::createParser(ShellType sn) {
  switch (sn) {
    case ShellType::FourNode: {
      shell_type_ = sn;
      generic_elem_ = element_factory_.createElement(shell_type_);
    } break;
  }
}
