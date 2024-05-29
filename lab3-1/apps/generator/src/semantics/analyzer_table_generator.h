#pragma once

// clang-format off
#include <boost/unordered_map.hpp>
// clang-format on

#include "ast.h"
#include "first_follow.h"

namespace semantics {

class AnalyzerTableGenerator final {
 public:
  using Key = std::pair<parser::Symbol, parser::Symbol>;
  using Value = std::pair<parser::SymbolVecIter, parser::SymbolVecIter>;

  AnalyzerTableGenerator(const FirstFollow& first_follow);

  void GenerateTable(const std::string& template_filename,
                     const std::string& table_filename) const;

 private:
  boost::unordered_map<Key, Value> table_;
  std::shared_ptr<const Program> program_;
};

}  // namespace semantics
