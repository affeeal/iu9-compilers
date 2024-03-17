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
  Scanner(std::shared_ptr<const std::string> program,
          std::shared_ptr<Compiler> compiler) noexcept
      : program_(std::move(program)),
        compiler_(std::move(compiler)),
        cur_(program_) {}

  DomainTag lex(Token* const yylval, Fragment* const yylloc);

 private:
  std::shared_ptr<const std::string> program_;
  std::shared_ptr<Compiler> compiler_;
  Position cur_;
};

std::unique_ptr<Scanner> GetScanner(
    const std::shared_ptr<const std::string>& program,
    const std::shared_ptr<Compiler>& compiler) noexcept;

}  // namespace lexer
