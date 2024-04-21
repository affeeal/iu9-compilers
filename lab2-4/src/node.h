#pragma once

#include <iterator>
#include <memory>
#include <vector>

// clang-format off
#include <boost/json.hpp>
// clang-format on

#include "token.h"

namespace parser {

class JsonSerializible {
 public:
  virtual ~JsonSerializible() = default;

  virtual boost::json::value ToJson() const = 0;
};

namespace ast {

class Pattern : virtual public JsonSerializible {
 public:
  virtual ~Pattern() = default;
};

class PatternBinary final : public Pattern {
  std::unique_ptr<Pattern> lhs_, rhs_;
  lexer::DomainTag op_;

 public:
  PatternBinary(std::unique_ptr<Pattern>&& lhs, std::unique_ptr<Pattern>&& rhs,
                const lexer::DomainTag op)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(op) {}

  boost::json::value ToJson() const override;
};

// TODO: collapse list to binary operations?
class PatternList final : public Pattern {
  std::vector<std::unique_ptr<Pattern>> patterns_;

 public:
  using PatternsIterator = decltype(patterns_)::iterator;

  PatternList(const std::move_iterator<PatternsIterator> begin,
              const std::move_iterator<PatternsIterator> end)
      : patterns_(begin, end) {}

  boost::json::value ToJson() const override;
};

class PatternTuple final : public Pattern {
  std::vector<std::unique_ptr<Pattern>> patterns_;

 public:
  using PatternsIterator = decltype(patterns_)::iterator;

  PatternTuple(const std::move_iterator<PatternsIterator> begin,
               const std::move_iterator<PatternsIterator> end)
      : patterns_(begin, end) {}

  boost::json::value ToJson() const override;
};

class Result : virtual public JsonSerializible {
 public:
  virtual ~Result() = default;
};

class ResultBinary final : public Result {
  std::unique_ptr<Result> lhs_, rhs_;
  lexer::DomainTag op_;

 public:
  ResultBinary(std::unique_ptr<Result>&& lhs, std::unique_ptr<Result>&& rhs,
               const lexer::DomainTag op)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(op) {}

  boost::json::value ToJson() const override;
};

// TODO: collapse list to binary operations?
class ResultList final : public Result {
  std::vector<std::unique_ptr<Result>> results_;

 public:
  using ResultsIterator = decltype(results_)::iterator;

  ResultList(const std::move_iterator<ResultsIterator> begin,
             const std::move_iterator<ResultsIterator> end)
      : results_(begin, end) {}

  boost::json::value ToJson() const override;
};

class ResultTuple final : public Result {
  std::vector<std::unique_ptr<Result>> results_;

 public:
  using ResultsIterator = decltype(results_)::iterator;

  ResultTuple(const std::move_iterator<ResultsIterator> begin,
              const std::move_iterator<ResultsIterator> end)
      : results_(begin, end) {}

  boost::json::value ToJson() const override;
};

class Var final : public Pattern, public Result {
  std::size_t ident_code_;

 public:
  Var(const std::size_t ident_code) : ident_code_(ident_code) {}

  boost::json::value ToJson() const override;
};

class IntConst final : public Pattern, public Result {
  std::int64_t value_;

 public:
  IntConst(const std::int64_t value) : value_(value) {}

  boost::json::value ToJson() const override;
};

class FuncCall final : public Result {
  std::unique_ptr<Result> arg_;
  std::size_t ident_code_;

 public:
  FuncCall(std::unique_ptr<Result>&& arg, const std::size_t ident_code)
      : arg_(std::move(arg)), ident_code_(ident_code) {}

  boost::json::value ToJson() const override;
};

class Statement final : public JsonSerializible {
  std::unique_ptr<Pattern> pattern_;
  std::unique_ptr<Result> result_;

 public:
  Statement(std::unique_ptr<Pattern>&& pattern,
            std::unique_ptr<Result>&& result)
      : pattern_(std::move(pattern)), result_(std::move(result)) {}

  boost::json::value ToJson() const override;
};

class FuncBody final : public JsonSerializible {
  std::vector<std::unique_ptr<Statement>> stmts_;

 public:
  using StmtsIterator = decltype(stmts_)::iterator;

  FuncBody(const std::move_iterator<StmtsIterator> begin,
           const std::move_iterator<StmtsIterator> end)
      : stmts_(begin, end) {}

  boost::json::value ToJson() const override;
};

class Type : public JsonSerializible {
 public:
  virtual ~Type() = default;
};

class ListType final : public Type {
  std::unique_ptr<Type> type_;

 public:
  ListType(std::unique_ptr<Type>&& type) : type_(std::move(type)) {}

  boost::json::value ToJson() const override;
};

class TupleType final : public Type {
  std::vector<std::unique_ptr<Type>> types_;

 public:
  using TypesIterator = decltype(types_)::iterator;

  TupleType(const std::move_iterator<TypesIterator> begin,
            const std::move_iterator<TypesIterator> end)
      : types_(begin, end) {}

  boost::json::value ToJson() const override;
};

class ElementaryType final : public Type {
  lexer::DomainTag tag_;

 public:
  ElementaryType(const lexer::DomainTag type) : tag_(type) {}

  boost::json::value ToJson() const override;
};

class FuncType final : public JsonSerializible {
  std::unique_ptr<Type> input_;
  std::unique_ptr<Type> output_;

 public:
  FuncType(std::unique_ptr<Type>&& input, std::unique_ptr<Type>&& output)
      : input_(std::move(input)), output_(std::move(output)) {}

  boost::json::value ToJson() const override;
};

class Func final : public JsonSerializible {
  std::unique_ptr<FuncType> type_;
  std::unique_ptr<FuncBody> body_;
  std::size_t ident_code_;

 public:
  Func(std::unique_ptr<FuncType>&& type, std::unique_ptr<FuncBody>&& body,
       const std::size_t ident_code)
      : type_(std::move(type)),
        body_(std::move(body)),
        ident_code_(ident_code) {}

  boost::json::value ToJson() const override;
};

class Program final : public JsonSerializible {
  std::vector<std::unique_ptr<Func>> funcs_;

 public:
  using FuncsIterator = decltype(funcs_)::iterator;

  Program(const std::move_iterator<FuncsIterator> begin,
          const std::move_iterator<FuncsIterator> end)
      : funcs_(begin, end) {}

  boost::json::value ToJson() const override;
};

}  // namespace ast

}  // namespace parser
