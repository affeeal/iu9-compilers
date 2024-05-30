#include "driver.h"

#include <fstream>

namespace frontend {

void Driver::Parse(const std::string& filename) {
  auto file = std::ifstream{filename};
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file " + filename);
  }

  auto scanner = Scanner{file, std::cout, &filename};
  scanner.set_debug(trace_scanning_);

  auto parser = Parser{scanner, *this};
  parser.set_debug_level(trace_parsing_);

  parser.parse();
}

}  // namespace frontend
