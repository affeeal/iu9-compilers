#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "ast.h"
#include "first_follow.h"
#include "parser.h"
#include "scanner.h"
#include "symbol_table.h"

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

    auto symbol_table = std::make_shared<parser::ast::SymbolTable>();
    const auto& node = static_cast<const parser::dt::InnerNode&>(*dt);
    const auto program = parser::ast::DtToAst(*symbol_table, node);

    parser::ast::Validate(*program);

    auto first_follow = parser::ast::FirstFollow(symbol_table, *program);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
