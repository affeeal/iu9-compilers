#include "parser.h"

#include <iterator>
#include <sstream>
#include <stdexcept>

#include "node.h"
#include "token.h"

namespace parser {

using lexer::DomainTag;

std::unique_ptr<ast::Program> Parser::RecursiveDescentParse() {
  sym_ = scanner_->NextToken();
  auto program = Program();
  Get(DomainTag::kEndOfProgram);
  return program;
}

/* Program ::= Func*. */
std::unique_ptr<ast::Program> Parser::Program() {
  auto funcs = std::vector<std::unique_ptr<ast::Func>>{};
  while (sym_->get_tag() == DomainTag::kIdent) {
    funcs.push_back(Func());
  }

  return std::make_unique<ast::Program>(std::make_move_iterator(funcs.begin()),
                                        std::make_move_iterator(funcs.end()));
}

/* Func ::= Ident FuncType 'is' FuncBody 'end'. */
std::unique_ptr<ast::Func> Parser::Func() {
  const auto ident = GetAndCast<lexer::IdentToken>(DomainTag::kIdent);
  auto type = FuncType();
  Get(DomainTag::kIs);
  auto body = FuncBody();
  Get(DomainTag::kEnd);

  return std::make_unique<ast::Func>(std::move(type), std::move(body),
                                     ident->get_code());
}

/* FuncType ::= Type '::' Type. */
std::unique_ptr<ast::FuncType> Parser::FuncType() {
  auto input = Type();
  Get(DomainTag::kColonColon);
  auto output = Type();

  return std::make_unique<ast::FuncType>(std::move(input), std::move(output));
}

/* Type ::= ElementaryType | ListType | TupleType. */
std::unique_ptr<ast::Type> Parser::Type() {
  switch (sym_->get_tag()) {
    case DomainTag::kInt: {
      return ElementaryType();
    }

    case DomainTag::kStar: {
      return ListType();
    }

    case DomainTag::kParanthesisLeft: {
      return TupleType();
    }

    default: {
      ThrowParseError(
          {DomainTag::kInt, DomainTag::kStar, DomainTag::kParanthesisLeft});
    }
  }
}

/* ElementaryType ::= 'int'. */
std::unique_ptr<ast::ElementaryType> Parser::ElementaryType() {
  Get(DomainTag::kInt);
  return std::make_unique<ast::ElementaryType>(DomainTag::kInt);
}

/* ListType ::= '*' Type. */
std::unique_ptr<ast::ListType> Parser::ListType() {
  Get(DomainTag::kStar);
  return std::make_unique<ast::ListType>(Type());
}

/* TupleType ::= '(' (Type (',' Type)*)? ')'. */
std::unique_ptr<ast::TupleType> Parser::TupleType() {
  Get(DomainTag::kParanthesisLeft);

  auto types = std::vector<std::unique_ptr<ast::Type>>{};
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kInt ||
                                        tag == DomainTag::kStar ||
                                        tag == DomainTag::kParanthesisLeft) {
    types.push_back(Type());

    while (sym_->get_tag() == DomainTag::kComma) {
      sym_ = scanner_->NextToken();
      types.push_back(Type());
    }
  }

  Get(DomainTag::kParanthesisRight);

  return std::make_unique<ast::TupleType>(
      std::make_move_iterator(types.begin()),
      std::make_move_iterator(types.end()));
}

std::unique_ptr<ast::FuncBody> Parser::FuncBody() {
  return nullptr;  // TODO!
}

template <typename T>
std::unique_ptr<T> Parser::GetAndCast(const DomainTag tag) {
  if (sym_->get_tag() != tag) {
    ThrowParseError({tag});
  }

  auto casted_sym = std::unique_ptr<T>{static_cast<T*>(sym_.release())};
  sym_ = scanner_->NextToken();
  return casted_sym;
}

void Parser::Get(const DomainTag tag) {
  if (sym_->get_tag() != tag) {
    ThrowParseError({tag});
  }

  sym_ = scanner_->NextToken();
}

[[noreturn]] void Parser::ThrowParseError(std::vector<DomainTag>&& expected) {
  std::ostringstream oss;
  oss << sym_->get_coords() << ": expected ";

  for (const auto tag : expected) {
    oss << tag << ", ";
  }

  oss << "got " << sym_->get_tag();
  throw std::runtime_error(oss.str());
}

}  // namespace parser
