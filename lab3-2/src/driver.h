#pragma once

#include <memory>

#include "ast.h"
#include "ident_table.h"
#include "scanner.h"

namespace fmt {

class Driver final {
  bool trace_scanning_, trace_parsing_;
  std::shared_ptr<Program> program_;
  std::shared_ptr<IdentTable> table_ = std::make_shared<IdentTable>();

 public:
  void Parse(const std::string& filename);

  void set_trace_scanning(const bool is_active) noexcept {
    trace_scanning_ = is_active;
  }
  void set_trace_parsing(const bool is_active) noexcept {
    trace_parsing_ = is_active;
  }

  void set_program(std::shared_ptr<Program>&& program) noexcept {
    program_ = std::move(program);
  }
  std::shared_ptr<const Program> get_program() const noexcept {
    return program_;
  }

  std::shared_ptr<IdentTable> get_ident_table() noexcept { return table_; }
  std::shared_ptr<const IdentTable> get_ident_table() const noexcept {
    return table_;
  }
};

}  // namespace fmt
