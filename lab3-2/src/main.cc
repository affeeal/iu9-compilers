#include <exception>
#include <iostream>

#include "driver.h"

int main(int argc, char* argv[]) try {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  auto driver = fmt::Driver{};
  driver.Parse(argv[1]);
  const auto* const program = driver.get_program();
} catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
}
