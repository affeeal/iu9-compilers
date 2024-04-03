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

WHITESPACE  [ \t\r\n]
N           n
PLUS        \+
STAR        \*
LPAREN      \(
RPAREN      \)

%%

{WHITESPACE}+  /* pass */
{N}            { return DomainTag::kN; }
{PLUS}         { return DomainTag::kPlus; }
{STAR}         { return DomainTag::kStar; }
{LPAREN}       { return DomainTag::kLparen; }
{RPAREN}       { return DomainTag::kRparen; }
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

}  // namespace lexer

#ifdef yylex
#undef yylex
#endif

int yyFlexLexer::yylex() {
  return 0;
}
