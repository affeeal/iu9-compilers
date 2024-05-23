#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "ast.h"
#include "first_set_generator.h"
#include "parser.h"
#include "scanner.h"
#include "validator.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
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
    const auto dt = parser.TopDownParse(scanner);
    const auto program =
        parser::ast::DtToAst(static_cast<parser::dt::InnerNode&>(*dt));
    auto validator = parser::ast::Validator();
    program->Accept(validator);

    auto f = parser::ast::FirstFollow(*program);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
