/**
 * @file node.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The node parser.
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "node.h"

#include <iomanip>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

namespace syntax {
namespace lsdyna {

Node::Node(std::string_view input) { parse(input); }

Node::Node(uint64_t t_id, double t_x, double t_y, double t_z) noexcept :
    id(t_id), x(t_x), y(t_y), z(t_z) {}

/**
 * @brief Node::parseNode implementation to import the nodes from the string.
 *
 * @details through regular expressions, it divides the input string from the
 * data to the node and adds them to the vector.
 *
 * @param pInputLine: line input from origin file.
 */
// PropertyNode<uint64_t, double> Node::parseNode(const QString &inputline) {
//   PropertyNode<uint64_t, double> node;
//   // clang-format off
//     QRegularExpression re;
//     // set pattern for search scheme of node definition
//     // extract coordinate z from string
//     ------------------------------------------------+
//     // extract coordinate y from string -----------------------+ |
//     // extract coordinate x from string ------+                | |
//     // extract node id ---+                   |                | |
//     //                    +                   +                + +
//     re.setPattern(R"((\d+) ?\s*
//     (-?\d+\.+\d+)?\s*(-?\d+\.+\d+)?\s*(-?\d+\.+\d+))");
//     QRegularExpressionMatch match = re.match(inputline);
//     qDebug().noquote() << INFOFILE << "fonud groups:" << re.captureCount();
//   // clang-format on
//   if (match.hasMatch()) {
//     // capture id node = number's node
//     auto id = static_cast<uint64_t>(match.captured(1).toInt());
//     // capture coordinate x of node
//     auto x = static_cast<double>(match.captured(2).toDouble());
//     // capture coordinate y of node
//     auto y = static_cast<double>(match.captured(3).toDouble());
//     // capture coordinate z of node
//     auto z = static_cast<double>(match.captured(4).toDouble());
//     // other field in input string are ignored
//     node = PropertyNode<uint64_t, double>(id, x, y, z);
//     qDebug().noquote() << INFOFILE << node;
//   }
//   return node;
// }

void Node::parse(std::string_view input) {
  auto str = std::string(input.data(), input.size());
  const std::regex re("[^\\s,]+");
  std::sregex_token_iterator it{str.begin(), str.end(), re, 0};
  std::vector<std::string> tokenized{it, {}};
  spdlog::debug("match {} elements", tokenized.size());
  if (tokenized.size() >= 4) {
    id = std::strtoull(tokenized[0].c_str(), nullptr, 10);
    x = std::stod(tokenized[1].c_str(), nullptr);
    y = std::stod(tokenized[2].c_str(), nullptr);
    z = std::stod(tokenized[3].c_str(), nullptr);
  }
}

std::ostream& operator<<(std::ostream& os, const Node& node) {
  os << node.id << " " << std::fixed << std::setprecision(7) << std::fixed
     << node.x << " " << std::fixed << std::setprecision(7) << node.y << " "
     << std::fixed << std::setprecision(7) << node.z;
  return os;
}

std::istream& operator>>(std::istream& is, Node& node) { return is; }

}  // namespace lsdyna
}  // namespace syntax
