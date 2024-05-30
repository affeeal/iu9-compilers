#pragma once

#include <memory>
#include <string>
#include <vector>

namespace semantics {

class Func;
class FuncType;
class FuncBody;
class IType;
class Statement;
class IPattern;
class IResult;

class Program final {
  std::vector<std::unique_ptr<Func>> funcs_;

 public:
  Program(std::vector<std::unique_ptr<Func>>&& funcs) noexcept
      : funcs_(std::move(funcs)) {}

  auto FuncsCbegin() const noexcept { return funcs_.cbegin(); }
  auto FuncsCend() const noexcept { return funcs_.cend(); }
};

class Func final {
  std::size_t ident_code_;
  std::unique_ptr<FuncType> type_;
  std::unique_ptr<FuncBody> body_;

 public:
  Func(std::unique_ptr<FuncType>&& type, std::unique_ptr<FuncBody>&& body,
       const std::size_t ident_code) noexcept
      : ident_code_(ident_code),
        type_(std::move(type)),
        body_(std::move(body)) {}

  std::size_t get_ident_code() const noexcept { return ident_code_; }
  const FuncType& get_type() const noexcept { return *type_; }
  const FuncBody& get_body() const noexcept { return *body_; }
};

class FuncType final {
  std::unique_ptr<IType> in_, out_;

 public:
  FuncType(std::unique_ptr<IType>&& in, std::unique_ptr<IType>&& out) noexcept
      : in_(std::move(in)), out_(std::move(out)) {}

  const IType& get_in() const noexcept { return *in_; }
  const IType& get_out() const noexcept { return *out_; }
};

class IType {
 public:
  virtual ~IType() = default;
};

class ElementaryType final : public IType {
 public:
  enum class Kind {
    kInt,
  };

 public:
  ElementaryType(const Kind kind) noexcept : kind_(kind) {}

  Kind get_kind() const noexcept { return kind_; }

 private:
  Kind kind_;
};

class ListType final : public IType {
  std::unique_ptr<IType> type_;

 public:
  ListType(std::unique_ptr<IType>&& type) noexcept : type_(std::move(type)) {}

  const IType& get_type() const noexcept { return *type_; }
};

class TupleType final : public IType {
  std::vector<std::unique_ptr<IType>> types_;

 public:
  TupleType(std::vector<std::unique_ptr<IType>>&& types) noexcept
      : types_(std::move(types)) {}

  auto TypesCbegin() const noexcept { return types_.cbegin(); }
  auto TypesCend() const noexcept { return types_.cend(); }
};

class FuncBody final {
  std::vector<std::unique_ptr<Statement>> statements_;

 public:
  FuncBody(std::vector<std::unique_ptr<Statement>>&& statements) noexcept
      : statements_(std::move(statements)) {}

  auto StatementsCbegin() const noexcept { return statements_.cbegin(); }
  auto StatementsCend() const noexcept { return statements_.cend(); }
};

class Statement final {
  std::unique_ptr<IPattern> pattern_;
  std::unique_ptr<IResult> result_;

 public:
  Statement(std::unique_ptr<IPattern>&& pattern,
            std::unique_ptr<IResult>&& result) noexcept
      : pattern_(std::move(pattern)), result_(std::move(result)) {}

  const IPattern& get_pattern() const noexcept { return *pattern_; }
  const IResult& get_result() const noexcept { return *result_; }
};

class IPattern {
 public:
  virtual ~IPattern() = default;
};

class PatternList final : public IPattern {
  std::vector<std::unique_ptr<IPattern>> patterns_;

 public:
  PatternList(std::vector<std::unique_ptr<IPattern>>&& patterns) noexcept
      : patterns_(std::move(patterns)) {}

  auto PatternsCbegin() const noexcept { return patterns_.cbegin(); }
  auto PatternsCend() const noexcept { return patterns_.cend(); }
};

class PatternTuple final : public IPattern {
  std::vector<std::unique_ptr<IPattern>> patterns_;

 public:
  PatternTuple(std::vector<std::unique_ptr<IPattern>>&& patterns) noexcept
      : patterns_(std::move(patterns)) {}

  auto PatternsCbegin() const noexcept { return patterns_.cbegin(); }
  auto PatternsCend() const noexcept { return patterns_.cend(); }
};

class PatternBinary final : public IPattern {
 public:
  enum class Op {
    kCons,
  };

 public:
  PatternBinary(std::unique_ptr<IPattern>&& lhs,
                std::unique_ptr<IPattern>&& rhs, const Op op) noexcept
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(op) {}

  const IPattern& get_lhs() const noexcept { return *lhs_; }
  const IPattern& get_rhs() const noexcept { return *rhs_; }
  Op get_op() const noexcept { return op_; }

 private:
  std::unique_ptr<IPattern> lhs_, rhs_;
  Op op_;
};

class IResult {
 public:
  virtual ~IResult() = default;
};

class ResultList final : public IResult {
  std::vector<std::unique_ptr<IResult>> results_;

 public:
  ResultList(std::vector<std::unique_ptr<IResult>> results) noexcept
      : results_(std::move(results)) {}

  auto ResultsCbegin() const noexcept { return results_.cbegin(); }
  auto ResultsCend() const noexcept { return results_.cend(); }
};

class ResultTuple final : public IResult {
  std::vector<std::unique_ptr<IResult>> results_;

 public:
  ResultTuple(std::vector<std::unique_ptr<IResult>>&& results)
      : results_(std::move(results)) {}

  auto ResultsCbegin() const noexcept { return results_.cbegin(); }
  auto ResutlesCend() const noexcept { return results_.cend(); }
};

class ResultBinary final : public IResult {
 public:
  enum class Op {
    kCons,
    kAdd,
    kMul,
    kSub,
    kDiv,
  };

 public:
  ResultBinary(std::unique_ptr<IResult>&& lhs, std::unique_ptr<IResult>&& rhs,
               const Op op) noexcept
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(op) {}

  const IResult& get_lhs() const noexcept { return *lhs_; }
  const IResult& get_rhs() const noexcept { return *rhs_; }
  Op get_op() const noexcept { return op_; }

 private:
  std::unique_ptr<IResult> lhs_, rhs_;
  Op op_;
};

class FuncCall final : public IResult {
  std::size_t ident_code_;
  std::unique_ptr<IResult> arg_;

 public:
  FuncCall(std::unique_ptr<IResult>&& arg,
           const std::size_t ident_code) noexcept
      : ident_code_(ident_code), arg_(std::move(arg)) {}

  std::size_t get_ident_code() const noexcept { return ident_code_; }
  const IResult& get_arg() const noexcept { return *arg_; }
};

class CaseInst final : public IResult {
  std::size_t ident_code_;
  std::vector<std::unique_ptr<Statement>> statements_;

 public:
  CaseInst(std::vector<std::unique_ptr<Statement>> statements) noexcept
    : statements_(std::move(statements)) {}
  
  std::size_t get_ident_code() const noexcept { return ident_code_; }
  auto StatementsCbegin() const noexcept { return statements_.cbegin(); }
  auto StatementsCend() const noexcept { return statements_.cend(); }
};

class Ident final : public IPattern, public IResult {
  std::size_t code_;

 public:
  Ident(const std::size_t code) : code_(code) {}

  std::size_t get_code() const noexcept { return code_; }
};

class IConst : public IPattern, public IResult {
 public:
  virtual ~IConst() = default;
};

class IntConst final : public IConst {
  std::uint64_t value_;

 public:
  IntConst(const std::uint64_t value) noexcept : value_(value) {}

  std::uint64_t get_value() const noexcept { return value_; }
};

}  // namespace semantics
