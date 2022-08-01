#ifndef GENERIC_ELEMENT_H
#define GENERIC_ELEMENT_H

#include <initializer_list>
#include <memory>

#include <QString>
#include <QVector>

class alignas(std::max_align_t) Element {
 public:
  virtual ~Element() = 0;

  virtual std::unique_ptr<Element> clone() = 0;

  virtual void setId(quint64 id) = 0;
  virtual void setNodes(std::initializer_list<quint64> const &li) = 0;
  virtual void setThickness(qreal thickness) = 0;

  [[nodiscard]] virtual quint64 getId() const = 0;
  [[nodiscard]] virtual qreal getThickness() const = 0;
  [[nodiscard]] virtual QVector<quint64> getNodesId() const = 0;

  virtual void parseElement(const QString &inputline) = 0;

 protected:
  QVector<quint64> element_id_;
  quint64 id_;
  qreal thickness_;
};

#endif  // GENERIC_ELEMENT_H
