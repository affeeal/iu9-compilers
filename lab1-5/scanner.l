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

// ...

}  // namespace lexer

#ifdef yylex
#undef yylex
#endif

int yyFlexLexer::yylex() {
  return 0;
}
