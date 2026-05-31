#include "common/element_shell_four_node.hh"

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <spdlog/spdlog.h>

#include <cctype>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

void ShellFourNode::parse_element(std::string_view input_line) {
  const std::string line(input_line.data(), input_line.size());
  extract_node(line);
  extract_thickness(line);

  if (m_node_flag && m_thickness_flag) {
    m_node_flag = false;
    m_thickness_flag = false;
  }
}

void ShellFourNode::extract_node(const std::string& input_line) {
  if (input_line == "*ELEMENT_SHELL_THICKNESS") {
    return;
  }

  if (m_node_flag) {
    return;
  }

  constexpr std::string_view pattern{" "};
#if __cpp_lib_ranges_to_container
  auto elems =
      input_line | std::ranges::views::split(pattern) |
      std::ranges::views::filter(
          [](auto r) { return !r.empty() && !std::isspace(r); }) |
      std::views::transform([](auto r) { return std::string_view{r}; }) |
      std::ranges::to<std::vector>();
#else
  auto splitView =
      input_line | std::ranges::views::split(pattern) |
      std::ranges::views::filter([](auto r) { return !r.empty(); });
  std::vector<std::string> elems;
  for (auto&& sv : splitView) {
    std::string token;
    for (auto&& c : sv) {
      token += c;
    }
    if (token.empty() ||
        std::all_of(token.begin(), token.end(),
                    [](unsigned char ch) { return std::isspace(ch); })) {
      continue;
    }
    elems.emplace_back(std::move(token));
  }
#endif

  // if (std::ranges::distance(splitView) != 6) {
  // spdlog::warn("line does not match the regex:\n {}", input_line);
  //}

  // for (const auto& substr : splitView) {
  //   fmt::print("{}\n", substr);
  // }

  // const std::regex re(
  //    R"((?<id>\d+)\s+(?<card>\d+)\s(?<node1>\d+)\s(?<node2>\d+)\s(?<node3>\d+)\s(?<node4>\d+))");
  // clang-format off
  //  set pattern for search scheme of element definition
  //  extract node element 4 from string
 // -----------------------------------------------------------------+
  //  extract node element 3 from string
  //-------------------------------------------------+               |
  //  extract node element 2 from string --------------------------------+ |
 // |
  //  extract node element 1 from string ---------------+                | |
 // |
  //  extract element card ignored -----+               |                | |
//  |
  //  extract id -------+               |               |                | |
//  |
  //                    +               +               +                + +
 // +
  //re.setPattern(R"((?<id>\d+)\s+(?<card>\d+)\s(?<node1>\d+)\s(?<node2>\d+)\s(?<node3>\d+)\s(?<node4>\d+))");
  // clang-format on
  // verify captured groups
  // std::smatch match;  //= re.match(inputline);
  // if (std::regex_match(input_line, match, re)) {
  //  spdlog::debug("");
  // capture id element

  // if (match.size() == 5) {
  //    std::ssub_match base_sub_match = match[1];
  //    std::string base = base_sub_match.str();
  //  }

  // setId(static_cast<uint64_t>(match.captured("id").toInt()));
  //  capture node element
  //  auto first_node = static_cast<uint64_t>(match.captured("node1").toInt());
  //  auto second_node = static_cast<uint64_t>(match.captured("node2").toInt());
  //  auto third_node = static_cast<uint64_t>(match.captured("node3").toInt());
  //  auto fourth_node = static_cast<uint64_t>(match.captured("node4").toInt());
  // setNodes({first_node, second_node, third_node, fourth_node});
  if (std::ranges::distance(splitView) == 6) {
    m_node_flag = true;
  } else {
    m_node_flag = false;
  }
  //} else {
  // spdlog::warn("line does not match regex: \n{}", input_line);
  //}
}

void ShellFourNode::extract_thickness(const std::string& input_line) {
  if (input_line == "*ELEMENT_SHELL_THICKNESS") {
    return;
  }

  if (m_node_flag) {
    return;
  } else {
  }

  fmt::println("extracting thickness from line:\n {}", input_line);
  constexpr std::string_view delimiter{" "};
  std::vector<std::string> tokenized{};
  for (const auto& part :
       input_line | std::views::split(delimiter) |
           std::views::transform([](auto r) { return std::string_view{r}; })) {
    tokenized.emplace_back(part);
    fmt::print("->{}\n", part);
  }
  // std::sregex_token_iterator it{input_line.begin(), input_line.end(), re, 0};
  // std::vector<std::string> tokenized{};
  if (!tokenized.empty()) {
    spdlog::debug("match {} thickness", tokenized.size());
    m_thickness = std::stod(tokenized.at(1), nullptr);
    m_thickness_flag = true;
  } else {
    spdlog::warn("line does not match regex to extract thickness:\n {}",
                 input_line);
  }
  spdlog::info("thickness element: {:7.13f}", m_thickness);
  // #if defined(__cpp_lib_ranges_to_container)
  // constexpr std::string_view delimiter{" "};
  //   auto tickness = input_line | std::views::split(delimiter) |
  //                   std::ranges::to<std::vector<double>>();
  // fmt::println("->{}", fmt::join(tickness, ", "));
  // #else

  // #endif

  // verify second line string element thickness replicated four times constant
  // for element
  // clang-format off
  //const std::regex re()
  //re.setPattern(R"((\d+?.\d+)       (\d+?.\d+)       (\d+?.\d+) (\d+?.\d+))");
  // clang-format on
  // match = re.match(input_line);
  // if (match.hasMatch()) {
  // clang-format off
  //  qDebug().noquote() << INFOFILE
   //         << "has match: " << match.hasMatch()
   //         << ", fonud groups:"
   //         << re.captureCount();
  // clang-format on
  // setThickness(match.captured(1).toDouble());
  // qDebug().noquote() << INFOFILE << "thickness element:" << getThickness();
  //  m_thickness_flag = true;
  //} else {

  // spdlog::warn("line does not match regex:\n {}", input_line);
  // }
  //   fill vector element then reset the flags
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
