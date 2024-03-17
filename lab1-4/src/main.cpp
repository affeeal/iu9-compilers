#include <algorithm>
#include <fstream>
#include <iostream>

#include "compiler.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: lab1-4 <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file " << argv[1] << "\n";
    return 1;
  }

  const auto program = std::make_shared<const std::string>(
      std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

  auto compiler = std::make_shared<lexer::Compiler>();
  auto scanner = lexer::GetScanner(compiler, program);

  std::vector<std::unique_ptr<lexer::Token>> tokens;

  do {
    tokens.push_back(scanner->NextToken());
  } while (tokens.back()->get_tag() != lexer::DomainTag::kEndOfProgram);

  std::cout << "TOKENS:\n";
  for (const auto& token : tokens) {
    std::cout << '\t';
    OutputToken(std::cout, token.get(), *compiler);
    std::cout << '\n';
  }

  std::cout << "COMMENTS:\n";
  for (const auto& coords : scanner->get_comments()) {
    std::cout << '\t' << coords << '\n';
  }

  compiler->OutputMessages(std::cout);
}
