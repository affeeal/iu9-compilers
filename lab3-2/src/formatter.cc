#include "formatter.h"

#include <cassert>
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
  FormatContainer(tuple_type.TypesCbegin(), tuple_type.TypesCend(), "(", ")");
}

void Formatter::Visit(const FuncBody& func_body) {
  FormatStatements(func_body.StatementsCbegin(), func_body.StatementsCend());
}

void Formatter::Visit(const Statement& statement) {
  BeginOfLine();
  statement.get_pattern().Accept(*this);
  oss_ << " = ";
  statement.get_result().Accept(*this);
}

void Formatter::Visit(const PatternList& list) {
  FormatContainer(list.PatternsCbegin(), list.PatternsCend(), "{", "}");
}

void Formatter::Visit(const PatternTuple& tuple) {
  FormatContainer(tuple.PatternsCbegin(), tuple.PatternsCend(), "(", ")");
}

void Formatter::Visit(const PatternBinary& pattern_binary) {
  pattern_binary.get_lhs().Accept(*this);
  oss_ << ' ' << pattern_binary.get_op() << ' ';
  pattern_binary.get_rhs().Accept(*this);
}

void Formatter::Visit(const ResultList& list) {
  FormatContainer(list.ResultsCbegin(), list.ResultsCend(), "{", "}");
}

void Formatter::Visit(const ResultTuple& tuple) {
  FormatContainer(tuple.ResultsCbegin(), tuple.ResultsCend(), "(", ")");
}

void Formatter::Visit(const ResultBinary& result_binary) {
  result_binary.get_lhs().Accept(*this);
  oss_ << ' ' << result_binary.get_op() << ' ';
  result_binary.get_rhs().Accept(*this);
}

void Formatter::Visit(const FuncCall& func_call) {
  oss_ << ident_table_->At(func_call.get_ident_code()) << ' ';
  func_call.get_arg().Accept(*this);
}

void Formatter::Visit(const CaseExpr& case_expr) {
  IndentIncreaseLn();
  BeginOfLine() << "case " << ident_table_->At(case_expr.get_ident_code())
                << " of";

  IndentIncreaseLn();
  FormatStatements(case_expr.StatementsCbegin(), case_expr.StatementsCend());
  IndentDecreaseLn();

  BeginOfLine() << "end";
  IndentDecrease();
}

void Formatter::Visit(const Ident& ident) {
  oss_ << ident_table_->At(ident.get_code());
}

void Formatter::Visit(const IntConst& int_const) {
  oss_ << int_const.get_value();
}

template <typename Iter>
requires std::input_iterator<Iter>
void Formatter::FormatContainer(Iter first, const Iter last,
                                const std::string& start,
                                const std::string& end,
                                const std::string& delimiter) {
  oss_ << start;
  if (first == last) {
    oss_ << end;
    return;
  }

  const auto e_prev = last - 1;
  for (; first != e_prev; ++first) {
    first->get()->Accept(*this);
    oss_ << delimiter;
  }

  e_prev->get()->Accept(*this);
  oss_ << end;
}

void Formatter::FormatStatements(Formatter::StatementIter first,
                                 const Formatter::StatementIter last) {
  assert(first != last);

  const auto last_prev = last - 1;
  for (; first != last_prev; ++first) {
    first->get()->Accept(*this);
    oss_ << ";\n";
  }

  last_prev->get()->Accept(*this);
}

std::ostream& Formatter::BeginOfLine() { return oss_ << current_indent_; }

void Formatter::IndentIncrease() { current_indent_.append(kIndent); }

void Formatter::IndentDecrease() {
  static constexpr auto kIndentSize = kIndent.size();

  const auto cend = current_indent_.cend();
  current_indent_.erase(cend - kIndentSize, cend);
}

void Formatter::IndentIncreaseLn() {
  IndentIncrease();
  oss_ << '\n';
}

void Formatter::IndentDecreaseLn() {
  IndentDecrease();
  oss_ << '\n';
}

}  // namespace fmt
