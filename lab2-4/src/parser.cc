#include "parser.h"

#include <iterator>
#include <sstream>
#include <stdexcept>

#include "node.h"
#include "token.h"

namespace parser {

using lexer::DomainTag;

std::unique_ptr<Program> Parser::RecursiveDescentParse() {
  sym_ = scanner_->NextToken();
  auto program = ParseProgram();
  Consume(DomainTag::kEndOfProgram);
  return program;
}

// Program ::= Func*.
std::unique_ptr<Program> Parser::ParseProgram() {
  auto funcs = std::vector<std::unique_ptr<Func>>{};
  while (sym_->get_tag() == DomainTag::kIdent) {
    funcs.push_back(ParseFunc());
  }

  return std::make_unique<Program>(std::make_move_iterator(funcs.begin()),
                                   std::make_move_iterator(funcs.end()));
}

// Func ::= Ident FuncType 'is' FuncBody 'end'.
std::unique_ptr<Func> Parser::ParseFunc() {
  const auto ident = GetAndCast<lexer::IdentToken>(DomainTag::kIdent);
  auto type = ParseFuncType();
  Consume(DomainTag::kIs);
  auto body = ParseFuncBody();
  Consume(DomainTag::kEnd);

  return std::make_unique<Func>(std::move(type), std::move(body),
                                ident->get_code());
}

// FuncType ::= Type '::' Type.
std::unique_ptr<FuncType> Parser::ParseFuncType() {
  auto input = ParseType();
  Consume(DomainTag::kColonColon);
  auto output = ParseType();

  return std::make_unique<FuncType>(std::move(input), std::move(output));
}

// Type ::= ElementaryType | ListType | TupleType.
std::unique_ptr<Type> Parser::ParseType() {
  switch (sym_->get_tag()) {
    case DomainTag::kInt: {
      return ParseElementaryType();
    }
    case DomainTag::kStar: {
      return ParseListType();
    }
    case DomainTag::kParanthesisLeft: {
      return ParseTupleType();
    }
    default: {
      ThrowParseError(
          {DomainTag::kInt, DomainTag::kStar, DomainTag::kParanthesisLeft});
    }
  }
}

// ElementaryType ::= 'int'.
std::unique_ptr<ElementaryType> Parser::ParseElementaryType() {
  Consume(DomainTag::kInt);
  return std::make_unique<ElementaryType>(DomainTag::kInt);
}

// ListType ::= '*' Type.
std::unique_ptr<ListType> Parser::ParseListType() {
  Consume(DomainTag::kStar);
  return std::make_unique<ListType>(ParseType());
}

// TupleType ::= '(' (Type (',' Type)*)? ')'.
std::unique_ptr<TupleType> Parser::ParseTupleType() {
  auto types = std::vector<std::unique_ptr<Type>>{};

  Consume(DomainTag::kParanthesisLeft);
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kInt ||
                                        tag == DomainTag::kStar ||
                                        tag == DomainTag::kParanthesisLeft) {
    types.push_back(ParseType());
    while (sym_->get_tag() == DomainTag::kComma) {
      sym_ = scanner_->NextToken();
      types.push_back(ParseType());
    }
  }
  Consume(DomainTag::kParanthesisRight);

  return std::make_unique<TupleType>(std::make_move_iterator(types.begin()),
                                     std::make_move_iterator(types.end()));
}

// FuncBody ::= Statement (';' Statement)*.
std::unique_ptr<FuncBody> Parser::ParseFuncBody() {
  auto stmts = std::vector<std::unique_ptr<Statement>>{};

  stmts.push_back(ParseStatement());
  while (sym_->get_tag() == DomainTag::kSemicolon) {
    sym_ = scanner_->NextToken();
    stmts.push_back(ParseStatement());
  }

  return std::make_unique<FuncBody>(std::make_move_iterator(stmts.begin()),
                                    std::make_move_iterator(stmts.end()));
}

// Statement ::= Pattern '=' Result.
std::unique_ptr<Statement> Parser::ParseStatement() {
  auto pattern = ParsePattern();
  Consume(DomainTag::kEqual);
  auto result = ParseResult();

  return std::make_unique<Statement>(std::move(pattern), std::move(result));
}

// Pattern ::= PatternUnit (':' Pattern)?.
std::unique_ptr<Pattern> Parser::ParsePattern() {
  auto pattern_unit = ParsePatternUnit();
  if (sym_->get_tag() == DomainTag::kColon) {
    sym_ = scanner_->NextToken();
    return std::make_unique<PatternBinary>(std::move(pattern_unit),
                                           ParsePattern(), DomainTag::kColon);
  }

  return pattern_unit;
}

// PatternUnit ::= Ident | IntConst | PatternList | PatternTuple |
//                 '[' Pattern ']'.
std::unique_ptr<Pattern> Parser::ParsePatternUnit() {
  switch (sym_->get_tag()) {
    case DomainTag::kIdent: {
      const auto ident_code = SymAs<lexer::IdentToken>().get_code();
      sym_ = scanner_->NextToken();
      return std::make_unique<Var>(ident_code);
    }
    case DomainTag::kIntConst: {
      const auto value = SymAs<lexer::IntConstToken>().get_value();
      sym_ = scanner_->NextToken();
      return std::make_unique<IntConst>(value);
    }
    case DomainTag::kCurlyBracketLeft: {
      return ParsePatternList();
    }
    case DomainTag::kParanthesisLeft: {
      return ParsePatternTuple();
    }
    case DomainTag::kSquareBracketLeft: {
      sym_ = scanner_->NextToken();
      auto pattern = ParsePattern();
      Consume(DomainTag::kSquareBracketRight);
      return pattern;
    }
    default: {
      ThrowParseError({DomainTag::kIdent, DomainTag::kIntConst,
                       DomainTag::kCurlyBracketLeft,
                       DomainTag::kParanthesisLeft,
                       DomainTag::kSquareBracketLeft});
    }
  }
}

// TODO: unsugar pattern list to pattern binary operations.

// PatternList ::= '{' (Pattern (',' Pattern)*)? '}'.
std::unique_ptr<PatternList> Parser::ParsePatternList() {
  auto patterns = std::vector<std::unique_ptr<Pattern>>{};

  Consume(DomainTag::kCurlyBracketLeft);
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kIdent ||
                                        tag == DomainTag::kIntConst ||
                                        tag == DomainTag::kCurlyBracketLeft ||
                                        tag == DomainTag::kParanthesisLeft ||
                                        tag == DomainTag::kSquareBracketLeft) {
    patterns.push_back(ParsePattern());
    while (sym_->get_tag() == DomainTag::kComma) {
      sym_ = scanner_->NextToken();
      patterns.push_back(ParsePattern());
    }
  }
  Consume(DomainTag::kCurlyBracketRight);

  return std::make_unique<PatternList>(
      std::make_move_iterator(patterns.begin()),
      std::make_move_iterator(patterns.end()));
}

// PatternTuple ::= '(' (Pattern (',' Pattern)*)? ')'.
std::unique_ptr<PatternTuple> Parser::ParsePatternTuple() {
  auto patterns = std::vector<std::unique_ptr<Pattern>>{};

  Consume(DomainTag::kParanthesisLeft);
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kIdent ||
                                        tag == DomainTag::kIntConst ||
                                        tag == DomainTag::kCurlyBracketLeft ||
                                        tag == DomainTag::kParanthesisLeft ||
                                        tag == DomainTag::kSquareBracketLeft) {
    patterns.push_back(ParsePattern());
    while (sym_->get_tag() == DomainTag::kComma) {
      sym_ = scanner_->NextToken();
      patterns.push_back(ParsePattern());
    }
  }
  Consume(DomainTag::kParanthesisRight);

  return std::make_unique<PatternTuple>(
      std::make_move_iterator(patterns.begin()),
      std::make_move_iterator(patterns.end()));
}

// Result ::= ResultUnit (':' Result)?.
std::unique_ptr<Result> Parser::ParseResult() {
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

template <typename T>
const T& Parser::SymAs() const {
  return static_cast<const T&>(*sym_);
}

void Parser::Consume(const DomainTag tag) {
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
