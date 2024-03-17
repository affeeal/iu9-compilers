#include <FlexLexer.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "scanner.hpp"
#include "compiler.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: lab1-5 <filename>\n";
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
  auto scanner = lexer::GetScanner(program, compiler);

  std::vector<std::unique_ptr<lexer::Token>> tokens;

  lexer::DomainTag tag;
  do {
    tag = scanner->lex(nullptr, nullptr);
  } while (tag != lexer::DomainTag::kEndOfProgram);
}
