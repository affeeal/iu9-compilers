#pragma once

#include "node.h"
#include "scanner.h"
#include "token.h"

namespace parser {

class Parser final {
 public:
  std::unique_ptr<dt::INode> TopDownParse(lexer::IScanner& scanner);
};

}  // namespace parser
