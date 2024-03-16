#pragma once

#include <list>
#include <memory>

#include "compiler.hpp"
#include "fragment.hpp"

namespace lexer {

class Compiler;
class Token;

class Scanner final {
 public:
  Scanner(std::shared_ptr<const std::string> program,
          std::shared_ptr<Compiler> compiler) noexcept
      : program_(std::move(program)),
        compiler_(std::move(compiler)),
        cur_(program_) {}

  void OutputComments(std::ostream& os) const;

  std::unique_ptr<Token> NextToken();

 private:
  std::shared_ptr<const std::string> program_;
  std::shared_ptr<Compiler> compiler_;
  std::list<Fragment> comments_;
  Position cur_;
};

}  // namespace lexer
