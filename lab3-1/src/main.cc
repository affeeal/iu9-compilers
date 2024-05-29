#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

// clang-format off
#include <boost/program_options.hpp>
// clang-format on

#include "analyzer_table_generator.h"
#include "ast.h"
#include "dt_to_ast.h"
#include "first_follow.h"
#include "parser.h"
#include "scanner.h"

namespace po = boost::program_options;

namespace {

constexpr auto kHelpOption = "help";
constexpr auto kGrammarOption = "grammar";
constexpr auto kTemplateOption = "template";
constexpr auto kTableOption = "table";

}  // namespace

int main(int ac, char* av[]) try {
  po::options_description desc("Allowed options");
  // clang-format off
  desc.add_options()
      (kHelpOption, "produce help message")
      (kGrammarOption, po::value<std::string>(), "set grammar filename")
      (kTemplateOption, po::value<std::string>(), "set template filename")
      (kTableOption, po::value<std::string>(), "set table filename")
  ;
  // clang-format on

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.contains(kHelpOption)) {
    std::cout << desc << "\n";
    return 1;
  }

  std::string grammar_filename;
  if (const auto it = vm.find(kGrammarOption); it != vm.cend()) {
    grammar_filename = it->second.as<std::string>();
  } else {
    std::cerr << "Grammar filename must be set\n";
    return 1;
  }

  std::string template_filename;
  if (const auto it = vm.find(kTemplateOption); it != vm.cend()) {
    template_filename = it->second.as<std::string>();
  } else {
    template_filename = "templates/analyzer_table.cc";
  }

  std::string table_filename;
  if (const auto it = vm.find(kTableOption); it != vm.cend()) {
    table_filename = it->second.as<std::string>();
  } else {
    table_filename = "build/analyzer_table.cc";
  }

  std::ifstream file(grammar_filename);
  if (!file.is_open()) {
    std::cerr << "Cannot open file " << grammar_filename << "\n";
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
  generator.GenerateTable(template_filename, table_filename);
} catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
  return 1;
}
