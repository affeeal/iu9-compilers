#pragma once

namespace parser {

namespace ast {

class Program;
class Rule;
class Term;
class Symbol;

class IVisitor {
 public:
  virtual ~IVisitor() = default;

  virtual void Visit(const Program& program) = 0;
  virtual void Visit(const Rule& rule) = 0;
  virtual void Visit(const Term& term) = 0;
  virtual void Visit(const Symbol& symbol) = 0;
};

}  // namespace ast

}  // namespace parser
