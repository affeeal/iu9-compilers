#include <FlexLexer.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "scanner.h"
#include "token.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: lab2-3 <filename>\n";
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
  lexer::Attribute attr;

  do {
    tag = scanner->NextToken(attr, coords);

    switch (tag) {
      case lexer::DomainTag::kNonTerminal: {
        tokens.push_back(std::make_unique<lexer::NonTerminalToken>(
            std::move(*attr), coords));
        break;
      }

      case lexer::DomainTag::kTerminal: {
        tokens.push_back(
            std::make_unique<lexer::TerminalToken>(std::move(*attr), coords));
        break;
      }

      default: {
        tokens.push_back(std::make_unique<lexer::SpecToken>(tag, coords));
      }
    }
  } while (tag != lexer::DomainTag::kEndOfProgram);

  std::cout << "TOKENS:\n";
  for (const auto& token : tokens) {
    std::cout << '\t' << *token << '\n';
  }

  std::cout << "MESSAGES:\n";
  for (const auto& [pos, msg] : compiler->get_messages()) {
    std::cout << '\t' << msg.type << pos << ": " << msg.text << '\n';
  }

  std::cout << "COMMENTS:\n";
  for (auto&& coords : scanner->get_comments()) {
    std::cout << "\t" << coords << " comment\n";
  }
}
