#pragma once

#include "ast.h"
#include "ident_table.h"
#include "scanner.h"

namespace fmt {

class Driver final {
  bool trace_scanning_, trace_parsing_;
  std::unique_ptr<Program> program_;
  IdentTable ident_table_;

 public:
  void Parse(const std::string& filename);

  void set_trace_scanning(const bool is_active) noexcept {
    trace_scanning_ = is_active;
  }
  void set_trace_parsing(const bool is_active) noexcept {
    trace_parsing_ = is_active;
  }

  void set_program(std::unique_ptr<Program>&& program) noexcept {
    program_ = std::move(program);
  }
  Program* get_program() noexcept { return program_.get(); }
  const Program* get_program() const noexcept { return program_.get(); }

  IdentTable& get_ident_table() noexcept { return ident_table_; }
  const IdentTable& get_ident_table() const noexcept { return ident_table_; }
};

}  // namespace fmt
