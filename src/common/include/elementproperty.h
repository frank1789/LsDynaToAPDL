#ifndef GENERIC_ELEMENT_H
#define GENERIC_ELEMENT_H

#include <QDebug>
#include <QString>
#include <QVector>
#include <initializer_list>
#include <iomanip>
#include <memory>
#include <ostream>

class Element {
 public:
  virtual ~Element() = 0;

  virtual std::unique_ptr<Element> clone() = 0;

  virtual void setId(quint64 id) = 0;
  virtual void setNodes(std::initializer_list<quint64> const& li) = 0;
  virtual void setThickness(qreal thickness) = 0;

  virtual quint64 getId() const = 0;
  virtual qreal getThickness() const = 0;
  virtual QVector<quint64> getNodesId() const = 0;

  virtual void parseElement(const QString& inputline) = 0;

 protected:
  quint64 id_;
  QVector<quint64> element_id_;
  qreal thickness_;
};

inline Element::~Element() {}

#endif  // GENERIC_ELEMENT_H
