#include "automata.hpp"

#include <cassert>
#include <iostream>

namespace lexer {

Automata::State Automata::At(const Automata::State state,
                             const unsigned char ch) const {
  assert(0 <= ch && ch < Automata::kCharacters);

  const auto factor = factors_[ch];
  return transitions_[state][factor];
}

bool Automata::IsFinal(const State state) const {
  return (domain_tags_[state] != DomainTag::kNotFinal);
}

DomainTag Automata::GetTag(const State state) const {
  return domain_tags_[state];
}

}  // namespace lexer
