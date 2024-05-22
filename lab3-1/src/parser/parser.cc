#include "parser.h"

#include <stack>

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

const AnalyzerTable& AnalyzerTable::Instance() {
  static AnalyzerTable table{};
  return table;
}

using lexer::DomainTag;

AnalyzerTable::AnalyzerTable()
    : sfs_({
          {Nonterminal::kRules},                      // 0
          {Nonterminal::kRule, Nonterminal::kRules},  // 1
          {/* ε */},                                  // 2
          {Nonterminal::kRuleLHS, DomainTag::kOpArrow,
           Nonterminal::kRuleRHS},                                      // 3
          {DomainTag::kKwAxiom, DomainTag::kNonterminal},               // 4
          {DomainTag::kNonterminal},                                    // 5
          {Nonterminal::kExpr, DomainTag::kKwEnd},                      // 6
          {Nonterminal::kTerm, Nonterminal::kExpr1},                    // 7
          {DomainTag::kKwOr, Nonterminal::kTerm, Nonterminal::kExpr1},  // 8
          {Nonterminal::kSymbol, Nonterminal::kTerm1},                  // 9
          {DomainTag::kTerminal},                                       // 10
          {DomainTag::kKwEpsilon},                                      // 11
      }),
      um_({
          {{Nonterminal::kProgram, DomainTag::kNonterminal}, sfs_[0]},
          {{Nonterminal::kProgram, DomainTag::kKwAxiom}, sfs_[0]},
          {{Nonterminal::kProgram, DomainTag::kEndOfProgram}, sfs_[0]},
          {{Nonterminal::kRules, DomainTag::kNonterminal}, sfs_[1]},
          {{Nonterminal::kRules, DomainTag::kKwAxiom}, sfs_[1]},
          {{Nonterminal::kRules, DomainTag::kEndOfProgram}, sfs_[2]},
          {{Nonterminal::kRule, DomainTag::kNonterminal}, sfs_[3]},
          {{Nonterminal::kRule, DomainTag::kKwAxiom}, sfs_[3]},
          {{Nonterminal::kRuleLHS, DomainTag::kNonterminal}, sfs_[5]},
          {{Nonterminal::kRuleLHS, DomainTag::kKwAxiom}, sfs_[4]},
          {{Nonterminal::kRuleRHS, DomainTag::kNonterminal}, sfs_[6]},
          {{Nonterminal::kRuleRHS, DomainTag::kTerminal}, sfs_[6]},
          {{Nonterminal::kRuleRHS, DomainTag::kKwEpsilon}, sfs_[6]},
          {{Nonterminal::kExpr, DomainTag::kNonterminal}, sfs_[7]},
          {{Nonterminal::kExpr, DomainTag::kTerminal}, sfs_[7]},
          {{Nonterminal::kExpr, DomainTag::kKwEpsilon}, sfs_[7]},
          {{Nonterminal::kExpr1, DomainTag::kKwOr}, sfs_[8]},
          {{Nonterminal::kExpr1, DomainTag::kKwEnd}, sfs_[2]},
          {{Nonterminal::kTerm, DomainTag::kNonterminal}, sfs_[9]},
          {{Nonterminal::kTerm, DomainTag::kTerminal}, sfs_[9]},
          {{Nonterminal::kTerm, DomainTag::kKwEpsilon}, sfs_[11]},
          {{Nonterminal::kTerm1, DomainTag::kNonterminal}, sfs_[9]},
          {{Nonterminal::kTerm1, DomainTag::kTerminal}, sfs_[9]},
          {{Nonterminal::kTerm1, DomainTag::kKwOr}, sfs_[2]},
          {{Nonterminal::kTerm1, DomainTag::kKwEnd}, sfs_[2]},
          {{Nonterminal::kSymbol, DomainTag::kNonterminal}, sfs_[5]},
          {{Nonterminal::kSymbol, DomainTag::kTerminal}, sfs_[10]},
      }) {}

auto AnalyzerTable::Find(const Nonterminal nonterminal,
                         const DomainTag tag) const& {
  return um_.find({nonterminal, tag});
}

std::unique_ptr<dt::INode> Parser::TopDownParse(lexer::IScanner& scanner) {
  using StackItem =
      std::pair<dt::Symbol, std::reference_wrapper<dt::InnerNode>>;

  auto dummy = std::make_unique<dt::InnerNode>(Nonterminal::kDummy);

  auto stack = std::stack<StackItem>{};
  stack.push({{DomainTag::kEndOfProgram}, *dummy});
  stack.push({{Nonterminal::kProgram}, *dummy});

  auto token = scanner.NextToken();
  const auto cend = table_.Cend();

  do {
    auto& [symbol, parent] = stack.top();

    if (auto* tag = std::get_if<DomainTag>(&symbol)) {
      if (token->tag() != *tag) {
        ThrowParseError(*token, *tag);
      }

      stack.pop();
      parent.get().AddChild(std::make_unique<dt::LeafNode>(std::move(token)));
      token = scanner.NextToken();
    } else {
      const auto nonterminal = std::get<Nonterminal>(symbol);

      if (auto it = table_.Find(nonterminal, token->tag()); it != cend) {
        stack.pop();
        auto& child = static_cast<dt::InnerNode&>(parent.get().AddChild(
            std::make_unique<dt::InnerNode>(nonterminal)));

        const auto& sf = it->second.get();
        for (auto it = sf.Crbegin(), end = sf.Crend(); it != end; ++it) {
          stack.push({*it, child});
        }
      } else {
        ThrowParseError(*token, nonterminal);
      }
    }
  } while (!stack.empty());

  return std::move(dummy->Children().front());
}

}  // namespace parser
