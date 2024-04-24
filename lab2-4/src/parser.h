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
  std::unique_ptr<ast::Sentence> Sentence();
  std::unique_ptr<ast::Pattern> Pattern();
  std::unique_ptr<ast::Pattern> PatternUnit();
  template <typename Value>
  std::unique_ptr<ast::Const<Value>> Const();
  std::unique_ptr<ast::Pattern> PatternList();
  std::unique_ptr<ast::PatternBinary> PatternListItems();
  std::unique_ptr<ast::PatternTuple> PatternTuple();
  std::unique_ptr<ast::Result> Result();
  std::unique_ptr<ast::Result> ResultUnit();
  std::unique_ptr<ast::Result> Expr();
  std::unique_ptr<ast::Result> Term();
  std::unique_ptr<ast::Result> Factor();
  std::unique_ptr<ast::Result> Atom();
  std::unique_ptr<ast::Result> FuncArg();
  std::unique_ptr<ast::Result> ResultList();
  std::unique_ptr<ast::ResultBinary> ResultListItems();
  std::unique_ptr<ast::ResultTuple> ResultTuple();

  template <typename T>
  std::unique_ptr<T> ExpectAndCast(const lexer::DomainTag tag);
  void Expect(const lexer::DomainTag tag);
  template <typename T>
  std::unique_ptr<T> SymTo();
  [[noreturn]] void ThrowParseError(std::vector<lexer::DomainTag>&& expected);

  std::unique_ptr<lexer::IScanner> scanner_;
  std::unique_ptr<lexer::Token> sym_;
};

}  // namespace parser
