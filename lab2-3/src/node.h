#pragma once

namespace parser {

enum class Symbol {};

struct Node {
  virtual ~Node() {}
};

struct Term final : Node {};

struct NTerm final : Node {};

}  // namespace parser
