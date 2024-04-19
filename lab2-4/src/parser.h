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

  std::unique_ptr<Program> RecursiveDescentParse();

 private:
  std::unique_ptr<Program> ParseProgram();
  std::unique_ptr<Func> ParseFunc();
  std::unique_ptr<FuncType> ParseFuncType();
  std::unique_ptr<Type> ParseType();
  std::unique_ptr<ElementaryType> ParseElementaryType();
  std::unique_ptr<ListType> ParseListType();
  std::unique_ptr<TupleType> ParseTupleType();
  std::unique_ptr<FuncBody> ParseFuncBody();
  std::unique_ptr<Statement> ParseStatement();
  std::unique_ptr<Pattern> ParsePattern();
  std::unique_ptr<Pattern> ParsePatternUnit();
  std::unique_ptr<PatternList> ParsePatternList();
  std::unique_ptr<PatternTuple> ParsePatternTuple();
  std::unique_ptr<Result> ParseResult();
  // TODO: the rest

  template <typename T>
  std::unique_ptr<T> GetAndCast(const lexer::DomainTag tag);
  template <typename T>
  const T& SymAs() const;
  void Consume(const lexer::DomainTag tag);
  [[noreturn]] void ThrowParseError(std::vector<lexer::DomainTag>&& expected);

  std::unique_ptr<lexer::IScanner> scanner_;
  std::unique_ptr<lexer::Token> sym_;
};

}  // namespace parser
