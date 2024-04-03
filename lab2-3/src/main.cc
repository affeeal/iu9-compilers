#include <FlexLexer.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "scanner.h"

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

  auto compiler = std::make_shared<lexer::Compiler>();
  auto scanner = std::make_unique<lexer::Scanner>(compiler, file);

  std::vector<std::unique_ptr<lexer::Token>> tokens;

  lexer::DomainTag tag;
  lexer::Fragment coords;

  do {
    tag = scanner->lex(coords);
    tokens.push_back(std::make_unique<lexer::SpecToken>(tag, coords));
  } while (tag != lexer::DomainTag::kEndOfProgram);

  std::cout << "TOKENS:\n";
  for (const auto& token : tokens) {
    std::cout << '\t' << *token << '\n';
  }

  std::cout << "MESSAGES:\n";
  for (const auto& [pos, msg] : compiler->get_messages()) {
    std::cout << '\t' << msg.type << pos << ": " << msg.text << '\n';
  }
}
