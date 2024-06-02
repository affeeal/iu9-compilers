#pragma once

#include <memory>
#include <ostream>
#include <sstream>

#include "ident_table.h"
#include "visitor.h"

namespace fmt {

// The object is supposed to be disposable.
class Formatter final : public IVisitor {
  std::shared_ptr<const IdentTable> ident_table_;
  std::ostringstream oss_;
  std::string current_indent_ = "";

  static constexpr std::string_view kIndent = "  ";

 public:
  Formatter(std::shared_ptr<const IdentTable>&& ident_table) noexcept
      : ident_table_(std::move(ident_table)) {}

  std::string ToString() const;

  void Visit(const Program& program) override;
  void Visit(const Func& func) override;
  void Visit(const FuncType& func_type) override;
  void Visit(const ElementaryType& elementary_type) override;
  void Visit(const ListType& list_type) override;
  void Visit(const TupleType& tuple_type) override;
  void Visit(const FuncBody& func_body) override;
  void Visit(const Statement& statement) override;
  void Visit(const PatternList& pattern_list) override;
  void Visit(const PatternTuple& pattern_tuple) override;
  void Visit(const PatternBinary& pattern_binary) override;
  void Visit(const ResultList& result_list) override;
  void Visit(const ResultTuple& result_tuple) override;
  void Visit(const ResultBinary& result_binary) override;
  void Visit(const FuncCall& func_call) override;
  void Visit(const CaseExpr& case_expr) override;
  void Visit(const Ident& ident) override;
  void Visit(const IntConst& int_const) override;

 private:
  std::ostream& BeginOfLine();
  void IndentIncreaseLn();
  void IndentDecreaseLn();
};

}  // namespace fmt
