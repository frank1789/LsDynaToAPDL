#ifndef NODE_TYPE_IMPL_H
#define NODE_TYPE_IMPL_H

#include <QObject>
#include <QTextStream>
#include <cassert>
#include <ostream>
#include <type_traits>
#include <utility>

#include "logger_tools.h"

/**
 * @brief PropertyNode contains the information of a single node.
 * @details The information is divided into:
 *  - ID of the node that uniquely identifies it;
 *  - x coordinate;
 *  - y coordinate;
 *  - z coordinate.
 *
 * Use a template definition to be able to create multiple definitions
 * depending on the nodes used.
 * It is suggested to use:
 *  - integer data types to define:
 *      - node IDs
 *  - float data type to define
 *      - coordinates.
 *
 * @tparam N: type for node IDs
 * @tparam P: type for spatial coordinate
 */
template <class N, class P>
class PropertyNode : public QObject {
  static_assert(std::is_integral_v<N>,
                "N must be instantiated with integral template argument.");
  static_assert(std::is_arithmetic_v<P>,
                "P must be instantiated with integral or floating-point "
                "template argument.");

 public:
  /**
   * @brief Construct a new Property Node object
   *
   */
  constexpr PropertyNode() = default;

  /**
   * @brief Construct a new Property Node object
   *
   * @param id
   * @param x
   * @param y
   * @param z
   */
  explicit PropertyNode(N id, P x, P y, P z)
      : id_node_(id), coordinate_x_(x), coordinate_y_(y), coordinate_z_(z) {}

  /**
   * @brief Construct a new Property Node object
   *
   * @param o
   */
  PropertyNode(const PropertyNode &o)
      : id_node_(o.id_node_),
        coordinate_x_(o.coordinate_z_),
        coordinate_y_(o.coordinate_z_),
        coordinate_z_(o.coordinate_z_) {}

  /**
   * @brief Construct a new Property Node object
   *
   * @param o
   */
  PropertyNode(PropertyNode &&o) noexcept
      : id_node_(std::move(o.id_node_)),
        coordinate_x_(std::move(o.coordinate_z_)),
        coordinate_y_(std::move(o.coordinate_z_)),
        coordinate_z_(std::move(o.coordinate_z_)) {}

  /**
   * @brief
   *
   * @param o
   * @return PropertyNode&
   */
  PropertyNode &operator=(const PropertyNode &o) {
    id_node_ = o.id_node_;
    coordinate_x_ = o.coordinate_z_;
    coordinate_y_ = o.coordinate_z_;
    coordinate_z_ = o.coordinate_z_;
    return *this;
  }

  /**
   * @brief
   *
   * @param o
   * @return PropertyNode&
   */
  PropertyNode &operator=(PropertyNode &&o) noexcept {
    id_node_ = std::move(o.id_node_);
    coordinate_x_ = std::move(o.coordinate_z_);
    coordinate_y_ = std::move(o.coordinate_z_);
    coordinate_z_ = std::move(o.coordinate_z_);
    return *this;
  }

  bool operator==(const PropertyNode &rhs) const {
    return this->reflect() == rhs.reflect();
  }

  bool operator!=(const PropertyNode &rhs) const {
    return !(this->reflect() == rhs.reflect());
  }

  /**
   * @brief
   *
   * @return P
   */
  P coordinate_x() const { return coordinate_x_; }

  /**
   * @brief Set the Coordinate x object
   *
   * @param coordinate_x
   */
  void setCoordinate_x(const P &coordinate_x) { coordinate_x_ = coordinate_x; }

  /**
   * @brief
   *
   * @return P
   */
  P coordinate_y() const { return coordinate_y_; }

  /**
   * @brief Set the Coordinate y object
   *
   * @param coordinate_y
   */
  void setCoordinate_y(const P &coordinate_y) { coordinate_y_ = coordinate_y; }

  /**
   * @brief
   *
   * @return P
   */
  P coordinate_z() const { return coordinate_z_; }

  /**
   * @brief Set the Coordinate z object
   *
   * @param coordinate_z
   */
  void setCoordinate_z(const P &coordinate_z) { coordinate_z_ = coordinate_z; }

  /**
   * @brief
   *
   * @return N
   */
  N id_node() const { return id_node_; }

  /**
   * @brief Set the Id node object
   *
   * @param id_node
   */
  void setId_node(const N &id_node) { id_node_ = id_node; }

  template <class T, class U>
  friend QDebug &operator<<(QDebug &os,  const PropertyNode<T, U> &node);

 protected:
  auto reflect() const {
    return std::tie(id_node_, coordinate_x_, coordinate_y_, coordinate_z_);
  }

 private:
  N id_node_;      /**< uniquely identifies node IDs */
  P coordinate_x_; /**< x coordinate in the space */
  P coordinate_y_; /**< y coordinate in the space */
  P coordinate_z_; /**< z coordinate in the space */
};

template <class N, class P>
inline QDebug &operator<<(QDebug &os, const PropertyNode<N, P> &node) {
  os << "[" << node.id_node_ << ", " << node.coordinate_x_ << ", "
     << node.coordinate_y_ << ", " << node.coordinate_z_ << "]";
  return os;
}

#endif  // NODE_TYPE_IMPL_H
