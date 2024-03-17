#include <iostream>
#include <memory>

#include <FlexLexer.h>

static constexpr auto kEof = 0;

int yyFlexLexer::yywrap() { return 1; }

int main() {
  auto lexer = std::make_unique<yyFlexLexer>();
  while (lexer->yylex() != kEof);
}
