#include "follow_set_generator.h"

#include "ast.h"

namespace parser {

namespace ast {

void FollowSetGenerator::Visit(const Program& program) {
  follow_sets_[index_.axiom->get_name()].insert(Special::kDollar);

  for (auto f = program.RulesCbegin(), l = program.RulesCend(); f != l; ++f) {
    f->get()->Accept(*this);
  }

  // TODO
}

void FollowSetGenerator::Visit(const Rule& rule) {
  for (auto f = rule.TermsCbegin(), l = rule.TermsCend(); f != l; ++f) {
    f->get()->Accept(*this);
  }

  // TODO
}

void FollowSetGenerator::Visit(const Term& term) {
  // TODO
}

void FollowSetGenerator::Visit(const Symbol& symbol) {
  // TODO
}

}  // namespace ast

}  // namespace parser
