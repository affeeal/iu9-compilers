%{
#include "scanner.h"

#define yyterminate() return lexer::DomainTag::kEndOfProgram

#define YY_USER_ACTION AdjustCoords(coords);

using lexer::DomainTag;
%}

%option c++ noyywrap

WHITESPACE  [ \t\r\n]
COMMENT     #.*
NONTERMINAL [A-Za-z][A-Za-z0-9]*
TERMINAL    \"[^"\n]+\"
ARROW       ->
KW_AXIOM    'axiom
KW_EPSILON  'epsilon
KW_OR       'or
KW_END      'end

%%

{WHITESPACE}+  /* pass */
{NONTERMINAL}  { return HandleNonterminal(attr); }
{TERMINAL}     { return HandleTerminal(attr); }
{ARROW}        { return DomainTag::kArrow; }
{KW_AXIOM}     { return DomainTag::kKwAxiom; }
{KW_EPSILON}   { return DomainTag::kKwEpsilon; }
{KW_OR}        { return DomainTag::kKwOr; }
{KW_END}       { return DomainTag::kKwEnd; }
{COMMENT}      { comments_.emplace_back(coords.starting, coords.following); }
.              { throw std::runtime_error(
                     "unexpected symbol " + std::string{yytext}); }

%%

namespace lexer {

std::unique_ptr<Token> Scanner::NextToken() {
  Fragment coords;
  Attribute attr;

  const auto tag = Lex(attr, coords);
  switch (tag) {
    case DomainTag::kNonterminal: {
      return std::make_unique<NonterminalToken>(std::move(*attr), coords);
    }
    case DomainTag::kTerminal: {
      return std::make_unique<TerminalToken>(std::move(*attr), coords);
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

DomainTag Scanner::HandleNonterminal(Attribute& attr) const {
  attr = std::make_unique<std::string>(yytext);
  return DomainTag::kNonterminal;
}

DomainTag Scanner::HandleTerminal(Attribute& attr) const {
  attr = std::make_unique<std::string>(yytext + 1, yyleng - 2);
  return DomainTag::kTerminal;
}

}  // namespace lexer

int yyFlexLexer::yylex() {
  return 0;
}
