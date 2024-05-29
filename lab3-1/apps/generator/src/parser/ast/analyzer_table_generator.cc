#include "analyzer_table_generator.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// clang-format off
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
// clang-format on

#include "ast.h"
#include "first_follow.h"

namespace parser {

namespace ast {

namespace {

std::string Slurp(std::ifstream& in) {
  std::ostringstream oss;
  oss << in.rdbuf();
  return oss.str();
}

std::string GetSymbolTypeDefinition(const Symbol::Type type) {
  switch (type) {
    case Symbol::Type::kNonterminal: {
      return "Symbol::Type::kNonterminal";
    }
    case Symbol::Type::kTerminal: {
      return "Symbol::Type::kTerminal";
    }
    case Symbol::Type::kSpecial: {
      return "Symbol::Type::kSpecial";
    }
  }
}

std::string GetSymbolDefinition(const Symbol& symbol) {
  return boost::str(boost::format("{%1%, %2%}") %
                    std::quoted(symbol.get_name()) %
                    GetSymbolTypeDefinition(symbol.get_type()));
}

}  // namespace

AnalyzerTableGenerator::AnalyzerTableGenerator(const FirstFollow& first_follow)
    : program_(first_follow.get_program()) {
  for (auto b = program_->RulesCbegin(), e = program_->RulesCend(); b != e;
       ++b) {
    const auto& rule = **b;

    for (auto b = rule.TermsCbegin(), e = rule.TermsCend(); b != e; ++b) {
      const auto& term = **b;

      auto first_set =
          first_follow.GetFirstSet(term.SymbolsCbegin(), term.SymbolsCend());
      const auto is_epsilon_erased = first_set.erase(kEpsilon);

      for (auto&& symbol : first_set) {
        const auto [_, is_inserted] =
            table_.insert({{rule.get_lhs(), symbol},
                           {term.SymbolsCbegin(), term.SymbolsCend()}});
        // TODO: remove duplication (?), recheck
        if (!is_inserted) {
          throw std::runtime_error("Not LL(1) grammar");
        }
      }

      if (!is_epsilon_erased) {
        continue;
      }

      for (auto [b, e] = first_follow.GetFollowSet(rule.get_lhs()); b != e;
           ++b) {
        const auto [_, is_inserted] = table_.insert(
            {{rule.get_lhs(), *b}, {term.SymbolsCbegin(), term.SymbolsCend()}});
        if (!is_inserted) {
          throw std::runtime_error("Not LL(1) grammar");
        }
      }
    }
  }
}

void AnalyzerTableGenerator::GenerateTable(
    const std::string& template_filename,
    const std::string& table_filename) const {
  auto template_file = std::ifstream(template_filename);
  if (!template_file.is_open()) {
    throw std::runtime_error("Failed to open file " + template_filename);
  }

  auto table_file = std::ofstream(table_filename);
  if (!table_file.is_open()) {
    throw std::runtime_error("Failed to create file " + table_filename);
  }

  auto records = std::vector<std::string>{};
  records.reserve(table_.size());
  for (auto&& [key, value] : table_) {
    const auto [nonterminal, symbol] = key;
    auto [b, e] = value;

    auto symbols = std::vector<std::string>{};
    symbols.reserve(e - b);
    for (; b != e; ++b) {
      symbols.push_back(GetSymbolDefinition(*b));
    }

    auto record = boost::str(boost::format("{{%1%, %2%}, {%3%}}") %
                             GetSymbolDefinition(nonterminal) %
                             GetSymbolDefinition(symbol) %
                             boost::algorithm::join(symbols, ", "));
    records.push_back(std::move(record));
  }
  const auto table_definition = boost::str(
      boost::format("{%1%}") % boost::algorithm::join(records, ", "));

  auto fmter = boost::format(Slurp(template_file));
  fmter % GetSymbolDefinition(program_->get_axiom()) % table_definition;
  table_file << fmter.str();
}

}  // namespace ast

}  // namespace parser
