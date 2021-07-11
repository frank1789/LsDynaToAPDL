#include "elementfactory.h"

#include <QMutexLocker>

ElementFactory::~ElementFactory() {}

auto ShellFactory::createShellFourNode() const
    -> QSharedPointer<ShellFourNode> {
  return QSharedPointer<ShellFourNode>(new ShellFourNode());
}
