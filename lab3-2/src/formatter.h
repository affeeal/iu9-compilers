#pragma once

#include "visitor.h"

namespace fmt {

class Formatter final : public IVisitor {
 public:
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
};

}  // namespace fmt
