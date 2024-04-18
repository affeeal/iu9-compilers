#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "parser.h"
#include "scanner.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: lab2-4 <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file " << argv[1] << "\n";
    return 1;
  }

  auto compiler = std::make_shared<lexer::Compiler>();
  auto scanner = std::make_unique<lexer::Scanner>(compiler, file);
  scanner->SetDebug(true);
  auto parser = parser::Parser(std::move(scanner));

  try {
    const auto root = parser.RecursiveDescentParse();
    // TODO: output nodes
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
