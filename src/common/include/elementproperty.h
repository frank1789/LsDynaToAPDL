#ifndef PropertyElement_H
#define PropertyElement_H

#include <initializer_list>
#include <iomanip>
#include <ostream>

#include <QDebug>
#include <QString>
#include <QVector>

class PropertyElement {
 public:
  virtual ~PropertyElement() = 0;

  virtual void setId(quint64 id) = 0;
  virtual void setNodes(std::initializer_list<quint64> const& li) = 0;
  virtual void setThickness(qreal thickness) = 0;

  virtual quint64 getId() const = 0;
  virtual qreal getThickness() const = 0;
  virtual QVector<quint64> getNodesId() const = 0;

  virtual void parseElement(const QString& inputline) = 0;

  template <typename TR>
  TR getElements() const;

  virtual int size() const = 0;

 protected:
  quint64 id_;
  QVector<quint64> element_id_;
  qreal thickness_;
};

inline PropertyElement::~PropertyElement() {}

#endif  // PropertyElement_H