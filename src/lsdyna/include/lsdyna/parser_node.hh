#ifndef LSDYNA_TO_APDL_LSYDNA_PARSER_NODE_HH
#define LSDYNA_TO_APDL_LSYDNA_PARSER_NODE_HH

#include "lsdyna/parser.hh"

class ParserNode : public Parser {
 public:
  void parse(const std::string &line) const override {};
};

#endif  // LSDYNA_TO_APDL_LSYDNA_PARSER_NODE_HH
