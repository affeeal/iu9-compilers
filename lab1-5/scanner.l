%{

#include "scanner.hpp"

#define yyterminate() return lexer::DomainTag::kEndOfProgram

// ...

using lexer::DomainTag;

%}

%option c++
%option debug
%option noyywrap

WHITESPACE      [ \t\r\n]
LETTER          [a-zA-Z]
CAPITAL_LETTER  [A-Z]
DIGIT           [0-9]
SUBSTANCE       {CAPITAL_LETTER}{LETTER}*({CAPITAL_LETTER}{LETTER}*|{DIGIT}+)*
COEFFICIENT     {DIGIT}+
PLUS            \+
ARROW           \-\>

%%

{WHITESPACE}+  /* pass */

{SUBSTANCE}    return DomainTag::kSubstance;

{COEFFICIENT}  return DomainTag::kCoefficient;

{PLUS}         return DomainTag::kPlus;

{ARROW}        return DomainTag::kArrow;

.              /* pass */

%%

namespace lexer {

std::unique_ptr<Scanner> GetScanner(
    const std::shared_ptr<const std::string>& program,
    const std::shared_ptr<Compiler>& compiler) noexcept {
  return std::make_unique<Scanner>(program, compiler);
}

}  // namespace lexer

#ifdef yylex
#undef yylex
#endif

int yyFlexLexer::yylex() {
  return 0;
}
