#pragma once

#include "node.h"
#include "scanner.h"
#include "token.h"

namespace parser {

class Parser final {
 public:
  Parser(std::unique_ptr<lexer::IScanner>&& scanner)
      : scanner_(std::move(scanner)) {}

  Parser(const Parser& other) = delete;
  Parser& operator=(const Parser& other) = delete;

  std::unique_ptr<ast::Program> RecursiveDescentParse();

 private:
  std::unique_ptr<ast::Program> Program();
  std::unique_ptr<ast::Func> Func();
  std::unique_ptr<ast::FuncType> FuncType();
  std::unique_ptr<ast::Type> Type();
  std::unique_ptr<ast::ElementaryType> ElementaryType();
  std::unique_ptr<ast::ListType> ListType();
  std::unique_ptr<ast::TupleType> TupleType();
  std::unique_ptr<ast::FuncBody> FuncBody();
  // TODO: the rest

  template <typename T>
  std::unique_ptr<T> GetAndCast(const lexer::DomainTag tag);
  void Get(const lexer::DomainTag tag);
  [[noreturn]] void ThrowParseError(std::vector<lexer::DomainTag>&& expected);

  std::unique_ptr<lexer::IScanner> scanner_;
  std::unique_ptr<lexer::Token> sym_;
};

}  // namespace parser
