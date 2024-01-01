#include "keywords.h"

// #include <fmt/format.h>

// auto fmt::formatter<syntax::lsdyna::KeywordDyna>::format(
//     syntax::lsdyna::KeywordDyna key, format_context& ctx) const {
//   string_view name{};
//   using dyna = syntax::lsdyna::KeywordDyna;
//   switch (key) {
//     case dyna::Header:
//       name = "$";
//       break;
//     case dyna::KeyWord:
//       name = "KEYWORD";
//       break;
//     case dyna::Node:
//       name = "NODE";
//       break;
//     case dyna::ElementShell:
//       name = "ELEMENTSHELL";
//       break;
//     case dyna::ElementSolid:
//       name = "ELEMENTSOLID";
//       break;
//     case dyna::InitialStrainSolid:
//       name = "INITIALSTRAINSOLID";
//       break;
//     case dyna::InitialStressShell:
//       name = "INITIALSTRESSSHELL";
//       break;
//     case dyna::End:
//       name = "END";
//       break;
//   }
//   return formatter<string_view>::format(name, ctx);
// }
