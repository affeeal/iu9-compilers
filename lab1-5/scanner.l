%{

#include "scanner.hpp"

#define yyterminate() return lexer::DomainTag::kEndOfProgram

#define YY_USER_ACTION AdjustCoords(yylloc);

using lexer::DomainTag;
using lexer::MessageType;

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

{SUBSTANCE}    { return HandleSubstance(yylval); }

{COEFFICIENT}  { return HandleCoefficient(yylval); }

{PLUS}         { return DomainTag::kPlus; }

{ARROW}        { return DomainTag::kArrow; }

.              { compiler_->AddMessage(MessageType::kError, yylloc.starting,
                                       "unexpected character"); }

%%

namespace lexer {

Scanner::Scanner(std::shared_ptr<Compiler> compiler, std::istream& is,
                 std::ostream& os)
    : yyFlexLexer(is, os), compiler_(std::move(compiler)) {}

void Scanner::AdjustCoords(Fragment& yylloc) noexcept {
  yylloc.starting = cur_;

  for (std::size_t i = 0; i < yyleng; ++i) {
    if (yytext[i] == '\n') {
      ++cur_.line;
      cur_.pos = 1;
    } else {
      ++cur_.pos;
    }

    ++cur_.index;
  }

  yylloc.following = cur_;
}

DomainTag Scanner::HandleSubstance(Attribute& yylval) const {
  yylval = std::make_unique<std::string>(yytext);
  return DomainTag::kSubstance;
}

DomainTag Scanner::HandleCoefficient(Attribute& yylval) const {
  yylval = std::stoull(yytext);
  return DomainTag::kCoefficient;
}

}  // namespace lexer

#ifdef yylex
#undef yylex
#endif

int yyFlexLexer::yylex() {
  return 0;
}
