#include "table.h"

#include <functional>

#include "node.h"
#include "token.h"

namespace parser {

using lexer::DomainTag;

const AnalyzerTable& AnalyzerTable::Instance() {
  static AnalyzerTable table{};
  return table;
}

AnalyzerTable::AnalyzerTable()
    : sfs_({
          {{NonTerminal::kRules}},                        // 0
          {{NonTerminal::kRule}, {NonTerminal::kRules}},  // 1
          {/* ε */},                                      // 2
          {{NonTerminal::kRuleLHS},
           {DomainTag::kOpArrow},
           {NonTerminal::kRuleRHS}},                           // 3
          {{DomainTag::kKwAxiom}, {DomainTag::kNonTerminal}},  // 4
          {{DomainTag::kNonTerminal}},                         // 5
          {{NonTerminal::kExpr}, {DomainTag::kKwEnd}},         // 6
          {{NonTerminal::kTerm}, {NonTerminal::kExpr1}},       // 7
          {{DomainTag::kKwOr},
           {NonTerminal::kTerm},
           {NonTerminal::kExpr1}},                          // 8
          {{NonTerminal::kFactor}, {NonTerminal::kTerm1}},  // 9
          {{DomainTag::kTerminal}},                         // 10
          {{DomainTag::kKwEpsilon}},                        // 11
      }),
      data_({
          {{NonTerminal::kProgram, DomainTag::kNonTerminal}, sfs_[0]},
          {{NonTerminal::kProgram, DomainTag::kKwAxiom}, sfs_[0]},
          {{NonTerminal::kProgram, DomainTag::kEndOfProgram}, sfs_[0]},
          {{NonTerminal::kRules, DomainTag::kNonTerminal}, sfs_[1]},
          {{NonTerminal::kRules, DomainTag::kKwAxiom}, sfs_[1]},
          {{NonTerminal::kRules, DomainTag::kEndOfProgram}, sfs_[2]},
          {{NonTerminal::kRule, DomainTag::kNonTerminal}, sfs_[3]},
          {{NonTerminal::kRule, DomainTag::kKwAxiom}, sfs_[3]},
          {{NonTerminal::kRuleLHS, DomainTag::kNonTerminal}, sfs_[5]},
          {{NonTerminal::kRuleLHS, DomainTag::kKwAxiom}, sfs_[4]},
          {{NonTerminal::kRuleRHS, DomainTag::kNonTerminal}, sfs_[6]},
          {{NonTerminal::kRuleRHS, DomainTag::kTerminal}, sfs_[6]},
          {{NonTerminal::kRuleRHS, DomainTag::kKwEpsilon}, sfs_[6]},
          {{NonTerminal::kExpr, DomainTag::kNonTerminal}, sfs_[7]},
          {{NonTerminal::kExpr, DomainTag::kTerminal}, sfs_[7]},
          {{NonTerminal::kExpr, DomainTag::kKwEpsilon}, sfs_[7]},
          {{NonTerminal::kExpr1, DomainTag::kKwOr}, sfs_[8]},
          {{NonTerminal::kExpr1, DomainTag::kKwEnd}, sfs_[2]},
          {{NonTerminal::kTerm, DomainTag::kNonTerminal}, sfs_[9]},
          {{NonTerminal::kTerm, DomainTag::kTerminal}, sfs_[9]},
          {{NonTerminal::kTerm, DomainTag::kKwEpsilon}, sfs_[9]},
          {{NonTerminal::kTerm1, DomainTag::kNonTerminal}, sfs_[9]},
          {{NonTerminal::kTerm1, DomainTag::kTerminal}, sfs_[9]},
          {{NonTerminal::kTerm1, DomainTag::kKwEpsilon}, sfs_[9]},
          {{NonTerminal::kTerm1, DomainTag::kKwOr}, sfs_[2]},
          {{NonTerminal::kTerm1, DomainTag::kKwEnd}, sfs_[2]},
          {{NonTerminal::kFactor, DomainTag::kNonTerminal}, sfs_[5]},
          {{NonTerminal::kFactor, DomainTag::kTerminal}, sfs_[10]},
          {{NonTerminal::kFactor, DomainTag::kKwEpsilon}, sfs_[11]},
      }) {}

auto AnalyzerTable::Find(const NonTerminal nt, const DomainTag t) const& {
  return data_.find({nt, t});
}

auto AnalyzerTable::Cend() const& noexcept { return data_.cend(); }

std::unique_ptr<Node> TopDownParse(const lexer::Scanner& scanner,
                                   const AnalyzerTable& table) {
}

}  // namespace parser
