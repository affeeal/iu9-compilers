#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "analyzer_table_generator.h"
#include "ast.h"
#include "dt_to_ast.h"
#include "first_follow.h"
#include "parser.h"
#include "scanner.h"

int main(int argc, char* argv[]) try {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file " << argv[1] << "\n";
    return 1;
  }

  auto scanner = lexer::Scanner(file);
  auto parser = parser::Parser();

  const auto dt = parser.TopDownParse(scanner);
  const auto& program_node = static_cast<const parser::dt::InnerNode&>(*dt);

  auto dt_to_ast = parser::ast::DtToAst{};
  const auto program = dt_to_ast.Convert(program_node);

  const auto first_follow = parser::ast::FirstFollow(program);
  const auto generator = parser::ast::AnalyzerTableGenerator(first_follow);
  generator.PrintTable();
} catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
  return 1;
}
