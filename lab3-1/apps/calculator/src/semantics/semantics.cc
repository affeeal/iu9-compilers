#include "semantics.h"

#include "node.h"

namespace semantics {

namespace {

constexpr std::uint64_t kAdditionNeutral = 0;
constexpr std::uint64_t kMultiplicationNeutral = 1;

std::uint64_t ParseE1(const parser::InnerNode& e1);
std::uint64_t ParseT(const parser::InnerNode& t);
std::uint64_t ParseT1(const parser::InnerNode& t1);
std::uint64_t ParseF(const parser::InnerNode& f);

// 'axiom E -> T E1 'end
std::uint64_t ParseE(const parser::InnerNode& e) {
  const auto b = e.ChildrenCbegin();
  const auto t = ParseT(static_cast<const parser::InnerNode&>(**b));
  const auto e1 = ParseE1(static_cast<const parser::InnerNode&>(**(b + 1)));
  return t + e1;
}

// E1 -> "+" T E1 'or 'epsilon 'end
std::uint64_t ParseE1(const parser::InnerNode& e1) {
  const auto b = e1.ChildrenCbegin();
  if (b == e1.ChildrenCend()) {
    return kAdditionNeutral;
  }

  const auto t = ParseT(static_cast<const parser::InnerNode&>(**(b + 1)));
  const auto e1_rhs = ParseE1(static_cast<const parser::InnerNode&>(**(b + 2)));
  return t + e1_rhs;
}

// T -> F T1 'end
std::uint64_t ParseT(const parser::InnerNode& t) {
  const auto b = t.ChildrenCbegin();
  const auto f = ParseF(static_cast<const parser::InnerNode&>(**b));
  const auto t1 = ParseT1(static_cast<const parser::InnerNode&>(**(b + 1)));
  return f * t1;
}

// T1 -> "*" F T1 'or 'epsilon 'end
std::uint64_t ParseT1(const parser::InnerNode& t1) {
  const auto b = t1.ChildrenCbegin();
  if (b == t1.ChildrenCend()) {
    return kMultiplicationNeutral;
  }

  const auto f = ParseF(static_cast<const parser::InnerNode&>(**(b + 1)));
  const auto t1_rhs = ParseT1(static_cast<const parser::InnerNode&>(**(b + 2)));
  return f * t1_rhs;
}

// F -> "n" 'or "(" E ")" 'end
std::uint64_t ParseF(const parser::InnerNode& f) {
  const auto b = f.ChildrenCbegin();
  if (f.ChildrenCend() - b == 3) {
    return ParseE(static_cast<const parser::InnerNode&>(**(b + 1)));
  }

  const auto& leaf = static_cast<const parser::LeafNode&>(**b);
  const auto& number =
      static_cast<const lexer::NumberToken&>(*leaf.get_token());
  return number.get_value();
}

}  // namespace

std::uint64_t Interpret(const parser::InnerNode& e) { return ParseE(e); }

}  // namespace semantics
