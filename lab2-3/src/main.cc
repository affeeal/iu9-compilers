#include <FlexLexer.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "scanner.h"
#include "table.h"
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

  const auto root =
      parser::TopDownParse(*scanner, parser::AnalyzerTable::Instance());
  root->Output();
}
