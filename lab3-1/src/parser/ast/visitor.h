#pragma once

namespace parser {

namespace ast {

class Program;
class Rule;
class Term;
class Epsilon;
class Terminal;
class NonTerminal;

class IVisitor {
 public:
  ~IVisitor() = default;

 public:
  virtual void Visit(Program& program) = 0;
  virtual void Visit(Rule& rule) = 0;
  virtual void Visit(Term& term) = 0;
  virtual void Visit(Epsilon& epsilon) = 0;
  virtual void Visit(Terminal& terminal) = 0;
  virtual void Visit(NonTerminal& non_terminal) = 0;
};

}  // namespace ast

}  // namespace parser
