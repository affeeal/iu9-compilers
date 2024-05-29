#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "node.h"
#include "parser.h"
#include "scanner.h"
#include "semantics/semantics.h"

int main(int argc, char* argv[]) try {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Failed to open file " << argv[1] << "\n";
    return 1;
  }

  auto scanner = lexer::Scanner(file);
  auto parser = parser::Parser();

  const auto dt = parser.TopDownParse(scanner);
  const auto& e = static_cast<const parser::InnerNode&>(*dt);
  std::cout << semantics::Interpret(e) << std::endl;
} catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
  return 1;
}
