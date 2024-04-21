#pragma once

#ifndef YY_DECL
#define YY_DECL                                                \
  lexer::DomainTag lexer::Scanner::Lex(lexer::Attribute& attr, \
                                       lexer::Fragment& coords)
#endif

#include <memory>
#include <variant>
#include <vector>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "compiler.h"
#include "fragment.h"
#include "token.h"

namespace lexer {

using Attribute = std::variant<std::size_t, std::int64_t>;

class IScanner {
 public:
  virtual std::unique_ptr<Token> NextToken() = 0;

  virtual ~IScanner() = default;
};

class Scanner final : private yyFlexLexer, public IScanner {
 public:
  Scanner(std::shared_ptr<Compiler> compiler, std::istream& is = std::cin,
          std::ostream& os = std::cout)
      : yyFlexLexer(is, os), compiler_(std::move(compiler)) {}

  auto CommentsCbegin() const& noexcept { return comments_.cbegin(); }
  auto CommentsCend() const& noexcept { return comments_.cend(); }

  void SetDebug(const bool is_active) { set_debug(is_active); }

  std::unique_ptr<Token> NextToken() override {
    Fragment coords;
    Attribute attr;

    const auto tag = Lex(attr, coords);

    switch (tag) {
      case DomainTag::kIdent: {
        return std::make_unique<IdentToken>(std::get<std::size_t>(attr),
                                            coords);
      }

      case DomainTag::kIntConst: {
        return std::make_unique<IntConstToken>(std::get<std::int64_t>(attr),
                                               coords);
      }

      default: {
        return std::make_unique<SpecToken>(tag, coords);
      }
    }
  }

 private:
  DomainTag Lex(Attribute& attr, Fragment& coords);

  void AdjustCoords(Fragment& coords) noexcept {
    coords.starting = cur_;

    for (std::size_t i = 0, end = static_cast<std::size_t>(yyleng); i < end;
         ++i) {
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

  DomainTag HandleIdent(Attribute& attr) const {
    attr = compiler_->AddName(yytext);
    return DomainTag::kIdent;
  }

  DomainTag HandleIntConst(Attribute& attr) const {
    attr = std::stoll(yytext);
    return DomainTag::kIntConst;
  }

  std::shared_ptr<Compiler> compiler_;
  std::vector<Fragment> comments_;
  Position cur_;
};

}  // namespace lexer
