#include "elementfactory.h"

#include <QMutexLocker>

ElementFactory::~ElementFactory() {}

QSharedPointer<ShellFourNode> ShellFactory::createShellFourNode() const {
  return QSharedPointer<ShellFourNode>(new ShellFourNode);
}
