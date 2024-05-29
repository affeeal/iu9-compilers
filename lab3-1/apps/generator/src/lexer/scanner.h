#pragma once

#ifndef YY_DECL
#define YY_DECL                                                \
  lexer::DomainTag lexer::Scanner::Lex(lexer::Attribute& attr, \
                                       lexer::Fragment& coords)
#endif

#include <memory>
#include <vector>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "fragment.h"
#include "token.h"

namespace lexer {

using Attribute = std::unique_ptr<std::string>;

class IScanner {
 public:
  virtual ~IScanner() = default;

  virtual std::unique_ptr<Token> NextToken() = 0;
};

class Scanner final : private yyFlexLexer, public IScanner {
 public:
  Scanner(std::istream& is = std::cin, std::ostream& os = std::cout)
      : yyFlexLexer(is, os) {}

  auto CommentsCbegin() const noexcept { return comments_.cbegin(); }
  auto CommentsCend() const noexcept { return comments_.cend(); }

  std::unique_ptr<Token> NextToken() override;

 private:
  DomainTag Lex(Attribute& attr, Fragment& coords);

  void AdjustCoords(Fragment& coords) noexcept;

  DomainTag HandleNonterminal(Attribute& attr) const;
  DomainTag HandleTerminal(Attribute& attr) const;

 private:
  std::vector<Fragment> comments_;
  Position cur_;
};

}  // namespace lexer
