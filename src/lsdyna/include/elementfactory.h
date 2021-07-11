#ifndef ELEMENT_FACTORY_H
#define ELEMENT_FACTORY_H

#include <QSharedPointer>


#include "finite_element_types.h"

/**
 * @brief The Element class extrapolates elements from the file and manage
 * information.
 *
 */
class Element : public PropertyElement {
 public:
  virtual void parseElement(const QString &inputline) = 0;
};

class ElementFactory {
 public:
  virtual ~ElementFactory() = 0;
  virtual QSharedPointer<ShellFourNode> createShellFourNode() const = 0;
};

class ShellFactory : public ElementFactory {
 public:
  ~ShellFactory() override = default;
  QSharedPointer<ShellFourNode> createShellFourNode() const override;
};

class SolidFactory : public ElementFactory {
 public:
};

#endif  // ELEMENT_FACTORY_H
