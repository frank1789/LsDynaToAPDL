#ifndef ELEMENT_TYPE_IMPL_H
#define ELEMENT_TYPE_IMPL_H

#include <QDebug>
#include <QVector>
#include <initializer_list>
#include <iomanip>
#include <ostream>

/**
 * @brief The propelem struct defines the general appearance of the parameters
 * of an element.
 * @details The content of the structure represents the element defined by a
 * unique ID, by the number of nodes that constitute it. The thickness used for
 * shell-like elements.
 * A template formulation is used to make most of the definitions available to
 * the structure.
 * It is suggested to use:
 *  - Integer data types:
 *      - element IDs;
 *      - nodes that constitute it as identified by the ID;
 *      - number of nodes of the element;
 *  - Float Data type:
 *      - thickness.
 * @tparam N: type for elments IDs
 * @tparam EN: type for node IDs
 * @tparam size: sets the number of nodes of the element
 */
template <typename N, typename EN, typename T, int size>
class PropertyElement {
 public:
  explicit PropertyElement() { element_id_.reserve(size); };
  virtual ~PropertyElement() {}

  virtual void setId(N id) = 0;
  virtual void setNodes(std::initializer_list<EN> const& li) = 0;
  virtual void setThickness(T thickness) = 0;

  virtual N getId() const = 0;
  virtual T getThickness() const = 0;
  virtual QVector<EN> getNodesId() const = 0;

 protected:
  N id_;                   /**< uniquely identifies element IDs. */
  QVector<EN> element_id_; /**< ID of the node that constitutes the element. */
  T thickness_;
};

template <typename N, typename EN, typename T, int size>
class ShellElement : public PropertyElement<N, EN, T, size> {
 public:
  explicit ShellElement() : PropertyElement<N, EN, T, size>(){};
  ~ShellElement() override{};

  void setId(N id) override { this->id_ = id; }
  void setNodes(std::initializer_list<EN> const& li) override {
    for (auto e : li) {
      this->element_id_.push_back(e);
    }
  }
  void setThickness(T thickness) override { this->thickness_ = thickness; }

  N getId() const override { return this->id_; }
  T getThickness() const override { return this->thickness_; }

  QVector<EN> getNodesId() const override { return this->element_id_; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const ShellElement<N, EN, T, size>& s);

  friend QDebug& operator<<(QDebug& os, const ShellElement<N, EN, T, size>& s);
};

template <typename N, typename EN, typename T, int size>
std::ostream& operator<<(std::ostream& os,
                         const ShellElement<N, EN, T, size>& s) {
  os << "element:\n\t";
  os << "id: " << std::setw(15) << s.getId() << "\t";
  auto nodes = s.getNodesId();
  for (auto i = 0; i < nodes.size(); i++) {
    os << "node " << i + 1 << ":" << std::setw(15) << nodes[i];
  }
  os << "\n";
  os << "thickness: " << std::setprecision(7) << s.getThickness() << "\n";
  return os;
}

template <typename N, typename EN, typename T, int size>
QDebug& operator<<(QDebug& os, const ShellElement<N, EN, T, size>& s) {
  os << "element:\n\t";
  os << "id: " << s.getId() << "\t";
  auto nodes = s.getNodesId();
  for (auto i = 0; i < nodes.size(); i++) {
    os << "node " << i + 1 << ":" << nodes[i];
  }
  os << "\n";
  os << "thickness: " << s.getThickness() << "\n";
  return os;
}

#endif  // ELEMENT_TYPE_IMPL_H
