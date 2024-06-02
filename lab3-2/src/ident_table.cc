#include "ident_table.h"

#include <cassert>
#include <iostream>

namespace fmt {

std::size_t IdentTable::GetCode(const std::string& name) {
  if (const auto it = codes_.find(name); it != codes_.cend()) {
    return it->second;
  }

  const auto code = names_.size();
  codes_[name] = code;
  names_.push_back(name);
  return code;
}

const std::string& IdentTable::At(const std::size_t code) const {
  assert(code < names_.size());
  return names_.at(code);
}

}  // namespace fmt
