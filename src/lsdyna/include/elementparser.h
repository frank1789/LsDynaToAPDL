#ifndef ELEMENT_PARSER_H
#define ELEMENT_PARSER_H

#include <QMutex>
#include <QSharedPointer>

#include "elementfactory.h"
#include "elementproperty.h"
#include "unique_cast.h"

class ElementParser {
 public:
  static QSharedPointer<ElementParser> getInstance();
  ~ElementParser() = default;

  ElementParser(const ElementParser &) = delete;
  ElementParser &operator=(const ElementParser &) = delete;

  template <class T>
  T parseElement(const QString &l) {
    auto elem = dynamic_unique_cast<T>(std::move(generic_elem_));
    elem->parseElement(l);
    return *elem;
  }

  void createParser(ShellType sn);

 private:
  ElementParser() = default;
  static QSharedPointer<ElementParser> p_instance_;
  static QMutex mutex_;
  ElementFactory element_factory_{};

  std::unique_ptr<Element> generic_elem_{nullptr};
  ShellType shell_type_;
};

#endif  // ELEMENT_PARSER_H
