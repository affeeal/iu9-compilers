#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "parser.h"
#include "scanner.h"

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
  auto scanner = lexer::Scanner(compiler, file);
  auto parser = parser::Parser();

  try {
    const auto root = parser.TopDownParse(scanner);
    root->Output();
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
