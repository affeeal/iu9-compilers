#pragma once

#include <variant>

#include "message.hpp"

#ifndef YY_DECL
#define YY_DECL                                                  \
  lexer::DomainTag lexer::Scanner::lex(lexer::Attribute& yylval, \
                                       lexer::Fragment& yylloc)
#endif

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "compiler.hpp"
#include "token.hpp"

namespace lexer {

using Attribute = std::variant<std::uint64_t, std::unique_ptr<std::string>>;

class Scanner : public yyFlexLexer {
 public:
  Scanner(std::shared_ptr<Compiler> compiler, std::istream& is = std::cin,
          std::ostream& os = std::cout);

  DomainTag lex(Attribute& yylval, Fragment& yylloc);

 private:
  void AdjustCoords(Fragment& yylloc) noexcept;

  DomainTag HandleSubstance(Attribute& yylval) const;
  DomainTag HandleCoefficient(Attribute& yylval) const;

  std::shared_ptr<Compiler> compiler_;
  Position cur_;
};

}  // namespace lexer
