#pragma once

#include "node.h"
#include "scanner.h"

namespace parser {

class Parser final {
 public:
  std::unique_ptr<Program> Parse(lexer::IScanner& scanner);
};

}  // namespace parser
