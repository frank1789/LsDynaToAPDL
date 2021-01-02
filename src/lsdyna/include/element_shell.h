#ifndef ELEMENT_SHELL_H
#define ELEMENT_SHELL_H

#include "element.h"

/**
 * @brief The Shell class this is specifically designed for shell elements four
 * nodes.
 */
class Shell : public Element {
public:
  /**
   * @brief Construct a new Shell object
   *
   */
  explicit Shell();
  ShellElement<quint64, quint64, qreal, 4>
  parseElement(const QString &inputline);

private:
  bool node_flag_;      /**< confirm extraction of node's connection. */
  bool thickness_flag_; /**< confirm extraction shell's thickness. */
};

Q_DECLARE_METATYPE(Shell)

#endif // ELEMENT_SHELL_H