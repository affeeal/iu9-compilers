#pragma once

#ifndef YY_DECL
#define YY_DECL                                                    \
  lexer::DomainTag lexer::Scanner::lex(lexer::Token* const yylval, \
                                       lexer::Fragment* const yylloc)
#endif

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "compiler.hpp"
#include "fragment.hpp"
#include "position.hpp"
#include "token.hpp"

namespace lexer {

class Scanner : public yyFlexLexer {
 public:
  Scanner(std::shared_ptr<Compiler> compiler, std::istream& is = std::cin,
          std::ostream& os = std::cout) noexcept
      : yyFlexLexer(is, os), compiler_(std::move(compiler)) {}

  DomainTag lex(Token* const yylval, Fragment* const yylloc);

 private:
  std::shared_ptr<Compiler> compiler_;
  Position cur_;
};

}  // namespace lexer
