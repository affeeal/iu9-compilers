#include "formatter.h"

#include <iostream>

#include "ast.h"

namespace fmt {

std::string Formatter::ToString() const { return oss_.str(); }

void Formatter::Visit(const Program& program) {
  if (program.FuncsCbegin() == program.FuncsCend()) {
    return;
  }

  const auto e_prev = program.FuncsCend() - 1;
  for (auto b = program.FuncsCbegin(); b != e_prev; ++b) {
    b->get()->Accept(*this);
    BeginOfLine() << "\n\n";
  }

  e_prev->get()->Accept(*this);
}

void Formatter::Visit(const Func& func) {
  BeginOfLine() << ident_table_->At(func.get_ident_code()) << ' ';
  func.get_type().Accept(*this);
  oss_ << " is";

  IndentIncreaseLn();
  func.get_body().Accept(*this);
  IndentDecreaseLn();

  BeginOfLine() << "end";
}

void Formatter::Visit(const FuncType& func_type) {
  func_type.get_in().Accept(*this);
  oss_ << " :: ";
  func_type.get_out().Accept(*this);
}

void Formatter::Visit(const ElementaryType& elementary_type) {
  oss_ << elementary_type.get_kind();
}

void Formatter::Visit(const ListType& list_type) {
  oss_ << "*";
  list_type.get_type().Accept(*this);
}

void Formatter::Visit(const TupleType& tuple_type) {
  oss_ << "(";
  if (tuple_type.TypesCbegin() == tuple_type.TypesCend()) {
    oss_ << ")";
    return;
  }

  const auto e_prev = tuple_type.TypesCend() - 1;
  for (auto b = tuple_type.TypesCbegin(); b != e_prev; ++b) {
    b->get()->Accept(*this);
    oss_ << ", ";
  }

  e_prev->get()->Accept(*this);
  oss_ << ")";
}

void Formatter::Visit(const FuncBody& func_body) {
  // TODO
}

void Formatter::Visit(const Statement& statement) {
  // TODO
}

void Formatter::Visit(const PatternList& pattern_list) {
  // TODO
}

void Formatter::Visit(const PatternTuple& pattern_tuple) {
  // TODO
}

void Formatter::Visit(const PatternBinary& pattern_binary) {
  // TODO
}

void Formatter::Visit(const ResultList& result_list) {
  // TODO
}

void Formatter::Visit(const ResultTuple& result_tuple) {
  // TODO
}

void Formatter::Visit(const ResultBinary& result_binary) {
  // TODO
}

void Formatter::Visit(const FuncCall& func_call) {
  // TODO
}

void Formatter::Visit(const CaseExpr& case_expr) {
  // TODO
}

void Formatter::Visit(const Ident& ident) {
  // TODO
}

void Formatter::Visit(const IntConst& int_const) {
  // TODO
}

std::ostream& Formatter::BeginOfLine() { return oss_ << current_indent_; }

void Formatter::IndentIncreaseLn() {
  current_indent_.append(kIndent);
  oss_ << '\n';
}

void Formatter::IndentDecreaseLn() {
  static constexpr auto kIndentSize = kIndent.size();

  const auto cend = current_indent_.cend();
  current_indent_.erase(cend - kIndentSize, cend);
  oss_ << '\n';
}

}  // namespace fmt
