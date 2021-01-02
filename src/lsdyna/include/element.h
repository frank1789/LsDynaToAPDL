#ifndef ELEMENT_H
#define ELEMENT_H

#include "finite_element_types.h"

/**
 * @brief The Element class extrapolates elements from the file and manage
 * information.
 *
 */
class Element {
public:
  template <class T> T parseElement(const QString &inputline);
};

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

Q_DECLARE_METATYPE(Element)
Q_DECLARE_METATYPE(Shell)

#endif // ELEMENT_H

// class Shell : public Element
// {
// public:
//     Shell();

//     ~Shell();

//     void readfromfile(QString pInputLine);

//     long size();

//     int getElementID(int i);

//     int getElementNode(int i, int j);

//     double getElementThickness(int i);

//     int getElementNumNode();

// private:
//     propelem<int, int, double, 4> _shelldata; /**< struct of shell to extract
//     information. */

//     QVector<propelem<int, int, double, 4>>* _shellimport; /**< vector
//     contains all shell extracted. */
