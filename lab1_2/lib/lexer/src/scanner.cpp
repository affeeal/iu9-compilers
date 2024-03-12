#include "scanner.hpp"

#include <boost/regex.hpp>
#include <string>

#include "message.hpp"
#include "position.hpp"
#include "token.hpp"

namespace lexer {

static const std::string kBinary = "BINARY";
static const std::string kDecimal = "DECIMAL";
static const std::string kIdent = "IDENT";
static const std::string kString = "STRING";

std::unique_ptr<Token> GetToken(Compiler& compiler, const Position& cur,
                                const boost::smatch& matches) {
  const auto str = matches[0].str();

  if (matches[kDecimal].matched) {
    const auto value = std::stoi(str);
    return std::make_unique<NumberToken>(cur, value);

  } else if (matches[kBinary].matched) {
    const auto value = std::stoi(str, nullptr, 2);
    return std::make_unique<NumberToken>(cur, value);

  } else if (matches[kIdent].matched) {
    const auto code = compiler.AddName(str);
    return std::make_unique<IdentToken>(cur, code);

  } else if (matches[kString].matched) {
    static const boost::regex double_back_quote("``");
    static constexpr std::string_view back_quote("`");

    std::ostringstream t;
    std::ostream_iterator<char> oi(t);
    boost::regex_replace(oi, str.begin() + 1, str.end() - 1, double_back_quote,
                         back_quote);
    return std::make_unique<StringToken>(cur, t.str());

  } else {
    throw std::runtime_error("scanner.cpp: undefined named subexpression");
  }
}

std::unique_ptr<Token> Scanner::NextToken() {
  // clang-format off
  static const boost::regex regex(
      "(?<" + kBinary + ">[01]+b)|"
      "(?<" + kDecimal + ">\\d+)|"
      "(?<" + kIdent + ">[\\?\\*\\|]([\\?\\*\\|]|\\d)*)|"
      "(?<" + kString + ">`([\\x00-\\x5F\\x61-\\x7F]|``)*`)");
  // clang-format on

  while (!cur_.IsEnd() && cur_.IsWhitespace()) {
    cur_.Next();
  }

  if (cur_.IsEnd()) {
    return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);
  }

  boost::smatch matches;
  if (!boost::regex_search(cur_.get_it(), program_->cend(), matches, regex)) {
    compiler_->AddMessage(MessageType::kError, cur_, kSyntaxError);
    return std::make_unique<SpecToken>(DomainTag::kEndOfProgram, cur_);
  }

  const auto& match = matches[0];

  if (cur_.get_it() != match.first) {
    compiler_->AddMessage(MessageType::kError, cur_, kSyntaxError);

    do {
      cur_.Next();
    } while (cur_.get_it() != match.first);
  }

  auto token = GetToken(*compiler_, cur_, matches);

  while (cur_.get_it() != match.second) {
    cur_.Next();
  }

  return token;
}

}  // namespace lexer
