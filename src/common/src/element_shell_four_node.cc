#include "common/element_shell_four_node.hh"

#include <regex>

#include "fmt/format.h"

ShellFourNode::ShellFourNode() noexcept :
    node_flag_(false), thickness_flag_(false) {}

void ShellFourNode::parse_element(std::string_view input_line) {
  extract_node(input_line);
  extract_thickness(input_line);
}

void ShellFourNode::extract_node(std::string_view input_line) {

  const std::regex re(R"((?<id>\d+)\s+(?<card>\d+)\s(?<node1>\d+)\s(?<node2>\d+)\s(?<node3>\d+)\s(?<node4>\d+))");
  // clang-format off
  //  set pattern for search scheme of element definition
  //  extract node element 4 from string
  -----------------------------------------------------------------+
  //  extract node element 3 from string
  -------------------------------------------------+               |
  //  extract node element 2 from string --------------------------------+ |
  |
  //  extract node element 1 from string ---------------+                | |
  |
  //  extract element card ignored -----+               |                | |
  |
  //  extract id -------+               |               |                | |
  |
  //                    +               +               +                + +
  +
  re.setPattern(R"((?<id>\d+)\s+(?<card>\d+)\s(?<node1>\d+)\s(?<node2>\d+)\s(?<node3>\d+)\s(?<node4>\d+))");
  // clang-format on
  // verify captured groups
  std::smatch match;//= re.match(inputline);
  if (std::regex_match(input_line, match, re)) {
    spdlod::debug("");
    // capture id element

    if (match.size() == 5)
{
                std::ssub_match base_sub_match = match[1];
                std::string base = base_sub_match.str();

            }










    //setId(static_cast<uint64_t>(match.captured("id").toInt()));
    // capture node element
    auto first_node = static_cast<uint64_t>(match.captured("node1").toInt());
    auto second_node = static_cast<uint64_t>(match.captured("node2").toInt());
    auto third_node = static_cast<uint64_t>(match.captured("node3").toInt());
    auto fourth_node = static_cast<uint64_t>(match.captured("node4").toInt());
    setNodes({first_node, second_node, third_node, fourth_node});
    node_flag_ = true;
  } else {
    spdlog::warn("line does not match regex: \n{}", input_line);
  }
}

  void ShellFourNode::extract_thickness(string_view input_line) {
  // verify second line string element thickness replicated four times constant for element
  // clang-format off
  re.setPattern(R"((\d+?.\d+)       (\d+?.\d+)       (\d+?.\d+) (\d+?.\d+))");
  // clang-format on
  match = re.match(inputline);
  if (match.hasMatch()) {
    // clang-format off
    qDebug().noquote() << INFOFILE
            << "has match: " << match.hasMatch()
            << ", fonud groups:"
            << re.captureCount();
    // clang-format on
    setThickness(match.captured(1).toDouble());
    qDebug().noquote() << INFOFILE << "thickness element:" << getThickness();
    thickness_flag_ = true;
  } else {

spdlod::warn("line does not match regex: \n {}" << input_line);
  }
  //  fill vector element then reset the flags
  if (node_flag_ == true && thickness_flag_ == true) {
    node_flag_ = false;
    thickness_flag_ = false;
  }
}

// void ShellFourNode::setId(uint64_t id) { id_ = id; }

// void ShellFourNode::setNodes(const std::initializer_list<uint64_t> &li) {
//   if (element_id_.size() == kFourNode) {
//     element_id_.clear();
//   }

//   for (auto e : li) {
//     element_id_.push_back(e);
//   }
// }

// void ShellFourNode::setThickness(double thickness) { thickness_ = thickness;
// }

// uint64_t ShellFourNode::getId() const { return id_; }

// double ShellFourNode::getThickness() const { return thickness_; }

// QVector<uint64_t> ShellFourNode::getNodesId() const { return element_id_; }

// std::unique_ptr<Element> ShellFourNode::clone() {
//   return std::make_unique<ShellFourNode>();
// }

// QDebug &operator<<(QDebug &os, const ShellFourNode &s) {
//   os << "element:\n\t";
//   os << "id: " << s.getId() << "\t";
//   auto nodes = s.getNodesId();
//   for (auto i = 0; i < nodes.size(); i++) {
//     os << "node " << i + 1 << ":" << nodes[i];
//   }
//   os << "\n";
//   os << "thickness: " << s.getThickness() << "\n";
//   return os;
// }
