#ifndef GENERIC_ELEMENT_H
#define GENERIC_ELEMENT_H

#include <initializer_list>
#include <memory>

class AbstractElement {
 public:
  virtual ~AbstractElement() noexcept = 0;

  AbstractElement(const AbstractElement&) = delete;
  AbstractElement(AbstractElement&&) noexcept = delete;

  AbstractElement& operator=(const AbstractElement&) = delete;
  AbstractElement& operator=(AbstractElement&&) noexcept = delete;

 protected:
  explicit AbstractElement() = default;
  AbstractElement(std::uint64_t id, std::vector<std::uint64_t> nodes,
                  double thickness);

  //   virtual std::unique_ptr<Element> clone() = 0;

  //   virtual void setId(uint64_t id) = 0;
  //   virtual void setNodes(std::initializer_list<uint64_t> const &li) = 0;
  //   virtual void setThickness(double thickness) = 0;

  [[nodiscard]] virtual std::uint64_t get_id() const = 0;
  [[nodiscard]] virtual double get_thickness() const = 0;
  // [[nodiscard]] virtual auto getNodesId() const = 0;

  //   virtual void parseElement(const QString &inputline) = 0;

  //  protected:
  //   QVector<uint64_t> element_id_;
  //   uint64_t id_;
  //   double thickness_;

 protected:
  std::uint64_t m_id{};
  double m_thickness{};
};

#endif  // GENERIC_ELEMENT_H
