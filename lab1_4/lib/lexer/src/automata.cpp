#include "automata.hpp"

namespace lexer {

void Automata::Dump(std::ostream& os) const {
  os << "FINAL STATES:\n";

  for (std::size_t i = 0; const auto is_final : final_states_) {
    os << '\t' << i++ << ": " << std::boolalpha << is_final << '\n';
  }

  os << "DOMAIN TAGS:\n";

  for (std::size_t i = 0; const auto tag : domain_tags_) {
    os << '\t' << i++ << ": " << std::boolalpha << ToString(tag) << '\n';
  }

  // ...
}

}  // namespace lexer
