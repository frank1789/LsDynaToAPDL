#ifndef GENERIC_ELEMENT_H
#define GENERIC_ELEMENT_H

#include <initializer_list>
#include <memory>


class alignas(std::max_align_t) Element {
 public:
  virtual ~Element() = 0;

//   virtual std::unique_ptr<Element> clone() = 0;

//   virtual void setId(uint64_t id) = 0;
//   virtual void setNodes(std::initializer_list<uint64_t> const &li) = 0;
//   virtual void setThickness(double thickness) = 0;

//   [[nodiscard]] virtual uint64_t getId() const = 0;
//   [[nodiscard]] virtual double getThickness() const = 0;
//   [[nodiscard]] virtual QVector<uint64_t> getNodesId() const = 0;

//   virtual void parseElement(const QString &inputline) = 0;

//  protected:
//   QVector<uint64_t> element_id_;
//   uint64_t id_;
//   double thickness_;
};

#endif  // GENERIC_ELEMENT_H
