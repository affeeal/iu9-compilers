#pragma once

namespace fmt {

class Program;
class Func;
class FuncType;
class ElementaryType;
class ListType;
class TupleType;
class FuncBody;
class Statement;
class PatternList;
class PatternTuple;
class PatternBinary;
class ResultList;
class ResultTuple;
class ResultBinary;
class FuncCall;
class CaseExpr;
class Ident;
class IntConst;

class IVisitor {
 public:
  virtual ~IVisitor() = default;

  virtual void Visit(const Program& program) = 0;
  virtual void Visit(const Func& func) = 0;
  virtual void Visit(const FuncType& func_type) = 0;
  virtual void Visit(const ElementaryType& elementary_type) = 0;
  virtual void Visit(const ListType& list_type) = 0;
  virtual void Visit(const TupleType& tuple_type) = 0;
  virtual void Visit(const FuncBody& func_body) = 0;
  virtual void Visit(const Statement& statement) = 0;
  virtual void Visit(const PatternList& pattern_list) = 0;
  virtual void Visit(const PatternTuple& pattern_tuple) = 0;
  virtual void Visit(const PatternBinary& pattern_binary) = 0;
  virtual void Visit(const ResultList& result_list) = 0;
  virtual void Visit(const ResultTuple& result_tuple) = 0;
  virtual void Visit(const ResultBinary& result_binary) = 0;
  virtual void Visit(const FuncCall& func_call) = 0;
  virtual void Visit(const CaseExpr& case_expr) = 0;
  virtual void Visit(const Ident& ident) = 0;
  virtual void Visit(const IntConst& int_const) = 0;
};

}  // namespace fmt
