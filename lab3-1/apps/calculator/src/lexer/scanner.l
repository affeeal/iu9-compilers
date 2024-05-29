%{
#include "scanner.h"

#define yyterminate() return lexer::DomainTag::kEndOfProgram

#define YY_USER_ACTION AdjustCoords(coords);

using lexer::DomainTag;
%}

%option c++ noyywrap

WHITESPACE [ \t\r\n]
NUMBER     [0-9][0-9]*

%%

{WHITESPACE}+ /* pass */
{NUMBER}      { return HandleNumber(attr); }
"+"           { return DomainTag::kPlus; }
"*"           { return DomainTag::kStar; }
"("           { return DomainTag::kLeftParenthesis; }
")"           { return DomainTag::kRightParenthesis; }
.             { throw std::runtime_error("unexpected character"); }

%%

namespace lexer {

std::unique_ptr<Token> Scanner::NextToken() {
  Fragment coords;
  Attribute attr;

  const auto tag = Lex(attr, coords);
  switch (tag) {
    case DomainTag::kNumber: {
      return std::make_unique<NumberToken>(attr, coords);
    }
    default: {
      return std::make_unique<SpecToken>(tag, coords);
    }
  }
}

void Scanner::AdjustCoords(Fragment& coords) noexcept {
  coords.starting = cur_;

  for (std::size_t i = 0, end = static_cast<std::size_t>(yyleng);
       i < end; ++i) {
    if (yytext[i] == '\n') {
      ++cur_.line;
      cur_.pos = 1;
    } else {
      ++cur_.pos;
    }

    ++cur_.index;
  }

  coords.following = cur_;
}

DomainTag Scanner::HandleNumber(Attribute& attr) const {
  attr = std::stoull(yytext);
  return DomainTag::kNumber;
}

}  // namespace lexer

int yyFlexLexer::yylex() {
  return 0;
}
