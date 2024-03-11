#pragma once

#include <memory>

#include "compiler.hpp"
#include "position.hpp"
#include "token.hpp"

namespace lexer {

class Compiler;

class Scanner final {
 public:
  Scanner(std::shared_ptr<const std::string> program,
          std::shared_ptr<Compiler> compiler) noexcept
      : program_(std::move(program)),
        compiler_(std::move(compiler)),
        cur_(program_) {}

  Token NextToken();

 private:
  std::shared_ptr<const std::string> program_;
  std::shared_ptr<Compiler> compiler_;
  Position cur_;
};

}  // namespace lexer
