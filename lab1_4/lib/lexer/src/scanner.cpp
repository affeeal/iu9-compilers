#include "scanner.hpp"
#include "automata.hpp"

#include <array>

namespace lexer {

static constexpr Automata automata;

std::unique_ptr<Token> Scanner::NextToken() {
  // TODO
}

}  // namespace lexer
