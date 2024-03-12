#include <algorithm>
#include <iostream>

#include "compiler.hpp"
#include "token.hpp"

int main() {
  auto compiler = std::make_shared<lexer::Compiler>();
  const auto program =
      std::make_shared<const std::string>(" \r\nc*1`a``\n``_0`101b123\t");

  std::vector<std::unique_ptr<lexer::Token>> tokens;
  auto scanner = lexer::GetScanner(compiler, program);

  do {
    tokens.push_back(scanner->NextToken());
  } while (tokens.back()->tag != lexer::DomainTag::kEndOfProgram);

  for (const auto& token : tokens) {
    lexer::Print(std::cout, *token, *compiler);
    std::cout << '\n';
  }

  compiler->OutputMessages();
}
