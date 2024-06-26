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
  Expect(DomainTag::kEndOfProgram);
  return program;
}

// Program ::= Func*.
std::unique_ptr<ast::Program> Parser::Program() {
  auto funcs = std::vector<std::unique_ptr<ast::Func>>{};
  while (sym_->get_tag() == DomainTag::kIdent) {
    funcs.push_back(Func());
  }

  return std::make_unique<ast::Program>(std::make_move_iterator(funcs.begin()),
                                        std::make_move_iterator(funcs.end()));
}

// Func ::= Ident FuncType 'is' FuncBody 'end'.
std::unique_ptr<ast::Func> Parser::Func() {
  const auto ident = ExpectAndCast<lexer::IdentToken>(DomainTag::kIdent);
  auto type = FuncType();
  Expect(DomainTag::kIs);
  auto body = FuncBody();
  Expect(DomainTag::kEnd);

  return std::make_unique<ast::Func>(std::move(type), std::move(body),
                                     ident->get_code());
}

// FuncType ::= Type '::' Type.
std::unique_ptr<ast::FuncType> Parser::FuncType() {
  auto input = Type();
  Expect(DomainTag::kColonColon);
  auto output = Type();

  return std::make_unique<ast::FuncType>(std::move(input), std::move(output));
}

// Type ::= ElementaryType | ListType | TupleType.
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

// ElementaryType ::= 'int'.
std::unique_ptr<ast::ElementaryType> Parser::ElementaryType() {
  Expect(DomainTag::kInt);
  return std::make_unique<ast::ElementaryType>(DomainTag::kInt);
}

// ListType ::= '*' Type.
std::unique_ptr<ast::ListType> Parser::ListType() {
  Expect(DomainTag::kStar);
  return std::make_unique<ast::ListType>(Type());
}

// TupleType ::= '(' (Type (',' Type)*)? ')'.
std::unique_ptr<ast::TupleType> Parser::TupleType() {
  auto types = std::vector<std::unique_ptr<ast::Type>>{};

  Expect(DomainTag::kParanthesisLeft);
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kInt ||
                                        tag == DomainTag::kStar ||
                                        tag == DomainTag::kParanthesisLeft) {
    types.push_back(Type());
    while (sym_->get_tag() == DomainTag::kComma) {
      sym_ = scanner_->NextToken();
      types.push_back(Type());
    }
  }
  Expect(DomainTag::kParanthesisRight);

  return std::make_unique<ast::TupleType>(
      std::make_move_iterator(types.begin()),
      std::make_move_iterator(types.end()));
}

// FuncBody ::= Sentence (';' Sentence)*.
std::unique_ptr<ast::FuncBody> Parser::FuncBody() {
  auto sents = std::vector<std::unique_ptr<ast::Sentence>>{};

  sents.push_back(Sentence());
  while (sym_->get_tag() == DomainTag::kSemicolon) {
    sym_ = scanner_->NextToken();
    sents.push_back(Sentence());
  }

  return std::make_unique<ast::FuncBody>(std::make_move_iterator(sents.begin()),
                                         std::make_move_iterator(sents.end()));
}

// Sentence ::= Pattern '=' Result.
std::unique_ptr<ast::Sentence> Parser::Sentence() {
  auto pattern = Pattern();
  Expect(DomainTag::kEqual);
  auto result = Result();

  return std::make_unique<ast::Sentence>(std::move(pattern), std::move(result));
}

// Pattern ::= PatternUnit (':' Pattern)?.
std::unique_ptr<ast::Pattern> Parser::Pattern() {
  auto pattern = PatternUnit();
  if (sym_->get_tag() == DomainTag::kColon) {
    sym_ = scanner_->NextToken();
    return std::make_unique<ast::PatternBinary>(std::move(pattern), Pattern(),
                                                DomainTag::kColon);
  }

  return pattern;
}

// PatternUnit ::= Ident | Const | PatternList | PatternTuple |
//                 '[' Pattern ']'.
std::unique_ptr<ast::Pattern> Parser::PatternUnit() {
  switch (sym_->get_tag()) {
    case DomainTag::kIdent: {
      const auto ident = SymTo<lexer::IdentToken>();
      sym_ = scanner_->NextToken();
      return std::make_unique<ast::Var>(ident->get_code());
    }
    case DomainTag::kIntConst: {
      return Const<std::int64_t>();
    }
    case DomainTag::kCurlyBracketLeft: {
      return PatternList();
    }
    case DomainTag::kParanthesisLeft: {
      return PatternTuple();
    }
    case DomainTag::kSquareBracketLeft: {
      sym_ = scanner_->NextToken();
      auto pattern = Pattern();
      Expect(DomainTag::kSquareBracketRight);
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

// Const ::= IntConst.
template <typename Value>
std::unique_ptr<ast::Const<Value>> Parser::Const() {
  const auto int_const =
      ExpectAndCast<lexer::IntConstToken>(DomainTag::kIntConst);
  return std::make_unique<ast::Const<std::int64_t>>(int_const->get_value(),
                                                    DomainTag::kIntConst);
}

// PatternList ::= '{' PatternListItems? '}' .
std::unique_ptr<ast::Pattern> Parser::PatternList() {
  Expect(DomainTag::kCurlyBracketLeft);

  std::unique_ptr<ast::Pattern> pattern;
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kIdent ||
                                        tag == DomainTag::kIntConst ||
                                        tag == DomainTag::kCurlyBracketLeft ||
                                        tag == DomainTag::kParanthesisLeft ||
                                        tag == DomainTag::kSquareBracketLeft) {
    pattern = PatternListItems();
  } else {
    pattern = std::make_unique<ast::EmptyList>();
  }

  Expect(DomainTag::kCurlyBracketRight);

  return pattern;
}

// PatternListItems ::= Pattern (',' PatternListItems)? .
std::unique_ptr<ast::PatternBinary> Parser::PatternListItems() {
  auto lhs = Pattern();

  std::unique_ptr<ast::Pattern> rhs;
  if (sym_->get_tag() == DomainTag::kComma) {
    sym_ = scanner_->NextToken();
    rhs = PatternListItems();
  } else {
    rhs = std::make_unique<ast::EmptyList>();
  }

  return std::make_unique<ast::PatternBinary>(std::move(lhs), std::move(rhs),
                                              DomainTag::kColon);
}

// PatternTuple ::= '(' (Pattern (',' Pattern)*)? ')'.
std::unique_ptr<ast::PatternTuple> Parser::PatternTuple() {
  auto patterns = std::vector<std::unique_ptr<ast::Pattern>>{};

  Expect(DomainTag::kParanthesisLeft);
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kIdent ||
                                        tag == DomainTag::kIntConst ||
                                        tag == DomainTag::kCurlyBracketLeft ||
                                        tag == DomainTag::kParanthesisLeft ||
                                        tag == DomainTag::kSquareBracketLeft) {
    patterns.push_back(Pattern());
    while (sym_->get_tag() == DomainTag::kComma) {
      sym_ = scanner_->NextToken();
      patterns.push_back(Pattern());
    }
  }
  Expect(DomainTag::kParanthesisRight);

  return std::make_unique<ast::PatternTuple>(
      std::make_move_iterator(patterns.begin()),
      std::make_move_iterator(patterns.end()));
}

// Result ::= ResultUnit (':' Result)?.
std::unique_ptr<ast::Result> Parser::Result() {
  auto result = ResultUnit();
  if (sym_->get_tag() == DomainTag::kColon) {
    sym_ = scanner_->NextToken();
    return std::make_unique<ast::ResultBinary>(std::move(result), Result(),
                                               DomainTag::kColon);
  }

  return result;
}

// ResultUnit ::= Expr | ResultList | ResultTuple.
std::unique_ptr<ast::Result> Parser::ResultUnit() {
  const auto tag = sym_->get_tag();
  if (tag == DomainTag::kIntConst || tag == DomainTag::kIdent ||
      tag == DomainTag::kSquareBracketLeft) {
    return Expr();
  } else if (tag == DomainTag::kCurlyBracketLeft) {
    return ResultList();
  } else if (tag == DomainTag::kParanthesisLeft) {
    return ResultTuple();
  } else {
    ThrowParseError(
        {DomainTag::kIntConst, DomainTag::kIdent, DomainTag::kSquareBracketLeft,
         DomainTag::kCurlyBracketLeft, DomainTag::kParanthesisLeft});
  }
}

// Expr ::= Term ('+' Term | '-' Term)*.
std::unique_ptr<ast::Result> Parser::Expr() {
  auto result = Term();
  for (auto tag = sym_->get_tag();
       tag == DomainTag::kPlus || tag == DomainTag::kMinus;
       tag = sym_->get_tag()) {
    sym_ = scanner_->NextToken();
    result =
        std::make_unique<ast::ResultBinary>(std::move(result), Term(), tag);
  }

  return result;
}

// Term ::= Factor ('*' Factor | '/' Factor)*.
std::unique_ptr<ast::Result> Parser::Term() {
  auto result = Factor();
  for (auto tag = sym_->get_tag();
       tag == DomainTag::kStar || tag == DomainTag::kSlash;
       tag = sym_->get_tag()) {
    sym_ = scanner_->NextToken();
    result =
        std::make_unique<ast::ResultBinary>(std::move(result), Factor(), tag);
  }

  return result;
}

// Factor ::= Atom | '[' Expr ']'.
std::unique_ptr<ast::Result> Parser::Factor() {
  const auto tag = sym_->get_tag();
  if (tag == DomainTag::kIntConst || tag == DomainTag::kIdent) {
    return Atom();
  } else if (tag == DomainTag::kSquareBracketLeft) {
    sym_ = scanner_->NextToken();
    auto expr = Expr();
    Expect(DomainTag::kSquareBracketRight);
    return expr;
  } else {
    ThrowParseError({DomainTag::kIntConst, DomainTag::kIdent,
                     DomainTag::kSquareBracketLeft});
  }
}

// Atom ::= Const | Ident FuncArg?.
std::unique_ptr<ast::Result> Parser::Atom() {
  const auto tag = sym_->get_tag();
  if (tag == DomainTag::kIntConst) {
    return Const<std::int64_t>();
  } else if (tag == DomainTag::kIdent) {
    const auto ident = SymTo<lexer::IdentToken>();
    sym_ = scanner_->NextToken();
    if (const auto tag = sym_->get_tag();
        tag == DomainTag::kIntConst || tag == DomainTag::kIdent ||
        tag == DomainTag::kCurlyBracketLeft ||
        tag == DomainTag::kParanthesisLeft ||
        tag == DomainTag::kSquareBracketLeft) {
      return std::make_unique<ast::FuncCall>(FuncArg(), ident->get_code());
    }
    return std::make_unique<ast::Var>(ident->get_code());
  } else {
    ThrowParseError({DomainTag::kIntConst, DomainTag::kIdent});
  }
}

// FuncArg ::= Atom | ResultList | ResultTuple | '[' Result ']'.
std::unique_ptr<ast::Result> Parser::FuncArg() {
  const auto tag = sym_->get_tag();
  if (tag == DomainTag::kIntConst || tag == DomainTag::kIdent) {
    return Atom();
  } else if (tag == DomainTag::kCurlyBracketLeft) {
    return ResultList();
  } else if (tag == DomainTag::kParanthesisLeft) {
    return ResultTuple();
  } else if (tag == DomainTag::kSquareBracketLeft) {
    sym_ = scanner_->NextToken();
    auto result = Result();
    Expect(DomainTag::kSquareBracketRight);
    return result;
  } else {
    ThrowParseError({DomainTag::kIntConst, DomainTag::kIdent,
                     DomainTag::kCurlyBracketLeft, DomainTag::kParanthesisLeft,
                     DomainTag::kSquareBracketLeft});
  }
}

// ResultList ::= '{' ResultListItems? '}' .
std::unique_ptr<ast::Result> Parser::ResultList() {
  Expect(DomainTag::kCurlyBracketLeft);

  std::unique_ptr<ast::Result> result;
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kIdent ||
                                        tag == DomainTag::kIntConst ||
                                        tag == DomainTag::kCurlyBracketLeft ||
                                        tag == DomainTag::kParanthesisLeft ||
                                        tag == DomainTag::kSquareBracketLeft) {
    result = ResultListItems();
  } else {
    result = std::make_unique<ast::EmptyList>();
  }

  Expect(DomainTag::kCurlyBracketRight);

  return result;
}

// ResultListItems ::= Result (',' ResultListItems)? .
std::unique_ptr<ast::ResultBinary> Parser::ResultListItems() {
  auto lhs = Result();

  std::unique_ptr<ast::Result> rhs;
  if (sym_->get_tag() == DomainTag::kComma) {
    sym_ = scanner_->NextToken();
    rhs = ResultListItems();
  } else {
    rhs = std::make_unique<ast::EmptyList>();
  }

  return std::make_unique<ast::ResultBinary>(std::move(lhs), std::move(rhs),
                                             DomainTag::kColon);
}

// ResultTuple ::= '(' (Result (',' Result)*)? ')'.
std::unique_ptr<ast::ResultTuple> Parser::ResultTuple() {
  auto results = std::vector<std::unique_ptr<ast::Result>>{};

  Expect(DomainTag::kParanthesisLeft);
  if (const auto tag = sym_->get_tag(); tag == DomainTag::kIntConst ||
                                        tag == DomainTag::kIdent ||
                                        tag == DomainTag::kSquareBracketLeft ||
                                        tag == DomainTag::kCurlyBracketLeft ||
                                        tag == DomainTag::kParanthesisLeft) {
    results.push_back(Result());
    while (sym_->get_tag() == DomainTag::kComma) {
      sym_ = scanner_->NextToken();
      results.push_back(Result());
    }
  }
  Expect(DomainTag::kParanthesisRight);

  return std::make_unique<ast::ResultTuple>(
      std::make_move_iterator(results.begin()),
      std::make_move_iterator(results.end()));
}

template <typename T>
std::unique_ptr<T> Parser::ExpectAndCast(const DomainTag tag) {
  if (sym_->get_tag() != tag) {
    ThrowParseError({tag});
  }

  auto casted_sym = SymTo<T>();
  sym_ = scanner_->NextToken();
  return casted_sym;
}

template <typename T>
std::unique_ptr<T> Parser::SymTo() {
  return std::unique_ptr<T>{static_cast<T*>(sym_.release())};
}

void Parser::Expect(const DomainTag tag) {
  if (sym_->get_tag() != tag) {
    ThrowParseError({tag});
  }

  sym_ = scanner_->NextToken();
}

[[noreturn]] void Parser::ThrowParseError(std::vector<DomainTag>&& expected) {
  std::ostringstream oss;
  oss << sym_->get_coords() << ": expected ";

  for (const auto tag : expected) {
    oss << lexer::ToString(tag) << ", ";
  }

  oss << "got " << lexer::ToString(sym_->get_tag());
  throw std::runtime_error(oss.str());
}

}  // namespace parser
