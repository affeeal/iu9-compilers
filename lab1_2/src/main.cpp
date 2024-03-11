#include <boost/regex.hpp>
#include <iostream>
#include <ostream>
#include <stdexcept>

#include "position.hpp"

namespace {

void Test(const std::string& text) {
  static const std::string kBinaryGroup = "BINARY";
  static const std::string kDecimalGroup = "DECIMAL";
  static const std::string kIdentifierGroup = "IDENTIFIER";
  static const std::string kStringGroup = "STRING";

  // clang-format off
  static const boost::regex regex(
      "(?<" + kBinaryGroup + ">[01]+b)|"
      "(?<" + kDecimalGroup + ">\\d+)|"
      "(?<" + kIdentifierGroup + ">[\\?\\*\\|]([\\?\\*\\|]|\\d)*)|"
      "(?<" + kStringGroup + ">`([\\x00-\\x5F\\x61-\\x7F]|``)*`)");
  // clang-format on

  lexer::Position p(text.cbegin(), text.cend());
  boost::smatch matches;

  while (true) {
    while (!p.IsEnd() && p.IsWhitespace()) {
      p.Next();
    }

    if (p.IsEnd()) {
      break;
    }

    if (!boost::regex_search(p.get_it(), p.get_end(), matches, regex)) {
      // TODO: handle syntax error
    }

    const auto& match = matches[0];

    if (matches[kDecimalGroup].matched) {
      // ...
    } else if (matches[kBinaryGroup].matched) {
      // ...
    } else if (matches[kIdentifierGroup].matched) {
      // ...
    } else if (matches[kStringGroup].matched) {
      // ...
    } else {
      throw std::runtime_error("lexer.hpp: undefined named subexpression");
    }

    p.set_it(match.second);
  }
}

}  // namespace

int main() {}
