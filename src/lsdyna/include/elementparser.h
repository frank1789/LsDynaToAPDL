#ifndef ELEMENT_PARSER_H
#define ELEMENT_PARSER_H

#include <QMutex>
#include <QSharedPointer>

#include "elementfactory.h"

class ElementParser {
 public:
  static QSharedPointer<ElementParser> getInstance();
  ~ElementParser() = default;

  ElementParser(const ElementParser &) = delete;
  ElementParser &operator=(const ElementParser &) = delete;

  void makeParser(ShellType sn, const ElementFactory &factory);
  void parseElement(const QString &l);

  QSharedPointer<PropertyElement> getGenericElement() const;

 private:
  ElementParser() = default;
  static QSharedPointer<ElementParser> p_instance_;
  static QMutex mutex_;
  QSharedPointer<PropertyElement> elem_{nullptr};
};

#endif  // ELEMENT_PARSER_H
