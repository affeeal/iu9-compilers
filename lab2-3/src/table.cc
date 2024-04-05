#include "table.h"

#include <concepts>
#include <functional>
#include <sstream>
#include <stack>
#include <stdexcept>

#include "node.h"
#include "token.h"

namespace parser {

namespace {

template <typename T>
concept Printable = requires(std::ostream& os, T&& t) {
  os << t;
};

template <Printable T>
void ThrowParseError(const lexer::Token& token, T&& t) {
  std::ostringstream err;
  err << token.coords() << ": expected " << t << ", got " << token.tag();
  throw std::runtime_error(err.str());
}

}  // namespace

using lexer::DomainTag;

const AnalyzerTable& AnalyzerTable::Instance() {
  static AnalyzerTable table{};
  return table;
}

AnalyzerTable::AnalyzerTable()
    : sfs_({
          {NonTerminal::kRules},                      // 0
          {NonTerminal::kRule, NonTerminal::kRules},  // 1
          {/* ε */},                                  // 2
          {NonTerminal::kRuleLHS, DomainTag::kOpArrow,
           NonTerminal::kRuleRHS},                                      // 3
          {DomainTag::kKwAxiom, DomainTag::kNonTerminal},               // 4
          {DomainTag::kNonTerminal},                                    // 5
          {NonTerminal::kExpr, DomainTag::kKwEnd},                      // 6
          {NonTerminal::kTerm, NonTerminal::kExpr1},                    // 7
          {DomainTag::kKwOr, NonTerminal::kTerm, NonTerminal::kExpr1},  // 8
          {NonTerminal::kFactor, NonTerminal::kTerm1},                  // 9
          {DomainTag::kTerminal},                                       // 10
          {DomainTag::kKwEpsilon},                                      // 11
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

auto AnalyzerTable::Find(const NonTerminal non_terminal,
                         const DomainTag tag) const& {
  return data_.find({non_terminal, tag});
}

auto AnalyzerTable::Cend() const& noexcept { return data_.cend(); }

std::unique_ptr<Node> TopDownParse(lexer::Scanner& scanner,
                                   const AnalyzerTable& table) {
  using StackItem = std::pair<Symbol, std::reference_wrapper<InnerNode>>;

  auto dummy_parent = std::make_unique<InnerNode>(NonTerminal::kDummy);

  auto stack = std::stack<StackItem>{};
  stack.push({{DomainTag::kEndOfProgram}, *dummy_parent});
  stack.push({{NonTerminal::kProgram}, *dummy_parent});

  auto token = scanner.NextToken();
  const auto cend = table.Cend();

  do {
    auto& [symbol, parent] = stack.top();

    if (auto* tag = std::get_if<DomainTag>(&symbol)) {
      if (token->tag() != *tag) {
        ThrowParseError(*token, *tag);
      }

      stack.pop();
      parent.get().AddChild(std::make_unique<LeafNode>(std::move(token)));
      token = scanner.NextToken();
    } else {
      const auto non_terminal = std::get<NonTerminal>(symbol);

      if (auto it = table.Find(non_terminal, token->tag()); it != cend) {
        stack.pop();
        auto& child = static_cast<InnerNode&>(
            parent.get().AddChild(std::make_unique<InnerNode>(non_terminal)));

        const auto& sf = it->second.get();
        for (auto it = sf.Crbegin(), end = sf.Crend(); it != end; ++it) {
          stack.push({*it, child});
        }
      } else {
        ThrowParseError(*token, non_terminal);
      }
    }
  } while (!stack.empty());

  return dummy_parent;
}

}  // namespace parser
