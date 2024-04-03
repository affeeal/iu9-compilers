#pragma once

#ifndef YY_DECL
#define YY_DECL lexer::DomainTag lexer::Scanner::lex(lexer::Fragment& yylloc)
#endif

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "compiler.hpp"
#include "message.hpp"
#include "token.hpp"

namespace lexer {

class Scanner : public yyFlexLexer {
 public:
  Scanner(std::shared_ptr<Compiler> compiler, std::istream& is = std::cin,
          std::ostream& os = std::cout);

  DomainTag lex(Fragment& yylloc);

 private:
  void AdjustCoords(Fragment& yylloc) noexcept;

  std::shared_ptr<Compiler> compiler_;
  Position cur_;
};

}  // namespace lexer
