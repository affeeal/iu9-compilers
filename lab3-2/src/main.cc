#include <exception>
#include <iostream>

#include "driver.h"
#include "formatter.h"

int main(int argc, char* argv[]) try {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  auto driver = fmt::Driver{};
  driver.Parse(argv[1]);

  auto formatter = fmt::Formatter{driver.get_ident_table()};
  driver.get_program()->Accept(formatter);
  std::cout << formatter.ToString() << std::endl;
} catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
}
