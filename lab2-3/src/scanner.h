#pragma once

#ifndef YY_DECL
#define YY_DECL                                                      \
  lexer::DomainTag lexer::Scanner::NextToken(lexer::Attribute& attr, \
                                             lexer::Fragment& coords)
#endif

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <vector>

#include "compiler.h"
#include "fragment.h"
#include "message.h"
#include "token.h"

namespace lexer {

using Attribute = std::unique_ptr<std::string>;

class Scanner : public yyFlexLexer {
 public:
  Scanner(std::shared_ptr<Compiler> compiler, std::istream& is = std::cin,
          std::ostream& os = std::cout)
      : yyFlexLexer(is, os), compiler_(std::move(compiler)) {}

  DomainTag NextToken(Attribute& attr, Fragment& coords);

  const std::vector<Fragment>& get_comments() const& noexcept {
    return comments_;
  }

 private:
  void AdjustCoords(Fragment& coords) noexcept;

  DomainTag HandleNonTerminal(Attribute& attr) const;
  DomainTag HandleTerminal(Attribute& attr) const;

  std::shared_ptr<Compiler> compiler_;
  std::vector<Fragment> comments_;
  Position cur_;
};

}  // namespace lexer
