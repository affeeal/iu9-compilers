#include "symbol.h"

// clang-format off
#include <boost/unordered_set.hpp>
// clang-format on

namespace parser {

std::size_t hash_value(const Symbol& symbol) {
  std::size_t seed = 0;
  boost::hash_combine(seed, symbol.get_type());
  boost::hash_combine(seed, symbol.get_name());
  return seed;
}

}  // namespace parser
