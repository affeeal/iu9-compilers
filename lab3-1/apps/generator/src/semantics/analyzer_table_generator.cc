#include "analyzer_table_generator.h"

#include <algorithm>
#include <filesystem>
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

namespace semantics {

namespace {

std::string Slurp(std::ifstream& in) {
  std::ostringstream oss;
  oss << in.rdbuf();
  return oss.str();
}

std::string GetSymbolTypeDefinition(const parser::Symbol::Type type) {
  switch (type) {
    case parser::Symbol::Type::kNonterminal: {
      return "Symbol::Type::kNonterminal";
    }
    case parser::Symbol::Type::kTerminal: {
      return "Symbol::Type::kTerminal";
    }
    case parser::Symbol::Type::kSpecial: {
      return "Symbol::Type::kSpecial";
    }
  }
  throw std::runtime_error("Unknown symbol type");
}

std::string GetSymbolDefinition(const parser::Symbol& symbol) {
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
      const auto is_epsilon_erased = first_set.erase(parser::kEpsilon);

      for (auto&& symbol : first_set) {
        const auto [_, is_inserted] =
            table_.insert({{rule.get_lhs(), symbol},
                           {term.SymbolsCbegin(), term.SymbolsCend()}});
        if (!is_inserted) {
          throw std::runtime_error("LL(1) conflict for nonterminal " +
                                   rule.get_lhs().get_name() +
                                   " and lookahead " + symbol.get_name());
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
          throw std::runtime_error("LL(1) conflict for nonterminal " +
                                   rule.get_lhs().get_name() +
                                   " and lookahead " + b->get_name());
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
  std::ranges::sort(records);
  const auto table_definition = boost::str(
      boost::format("{%1%}") % boost::algorithm::join(records, ", "));

  auto fmter = boost::format(Slurp(template_file));
  fmter % GetSymbolDefinition(program_->get_axiom()) % table_definition;
  const auto output = fmter.str();

  auto temporary_filename = std::filesystem::path(table_filename);
  temporary_filename += ".tmp";
  if (std::filesystem::exists(temporary_filename)) {
    throw std::runtime_error("Temporary output already exists: " +
                             temporary_filename.string());
  }

  try {
    auto table_file =
        std::ofstream(temporary_filename, std::ios::out | std::ios::trunc);
    if (!table_file.is_open()) {
      throw std::runtime_error("Failed to create file " +
                               temporary_filename.string());
    }
    table_file << output;
    table_file.close();
    if (!table_file) {
      throw std::runtime_error("Failed to write file " +
                               temporary_filename.string());
    }
    std::filesystem::rename(temporary_filename, table_filename);
  } catch (...) {
    std::error_code error;
    std::filesystem::remove(temporary_filename, error);
    throw;
  }
}

}  // namespace semantics
