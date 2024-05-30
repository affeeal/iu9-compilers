#pragma once

#include <iostream>
#include <ostream>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL \
  frontend::Parser::symbol_type frontend::Scanner::Get(frontend::Driver& driver)

#include "location.h"
#include "parser.h"

namespace frontend {

class Driver;

class Scanner final : public yyFlexLexer {
 public:
  Scanner(std::istream& is = std::cin, std::ostream& os = std::cout,
          const std::string* isname = nullptr)
      : yyFlexLexer(is, os), loc_(isname) {}

  Parser::symbol_type Get(Driver& driver);

 private:
  location loc_;
};

}  // namespace frontend
