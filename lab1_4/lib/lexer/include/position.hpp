#pragma once

#include <memory>

namespace lexer {

constexpr char kEnd = -1;

class Position final {
 public:
  Position(std::shared_ptr<const std::string> text) noexcept
      : text_(std::move(text)), line_(1), pos_(1), index_(0) {}

  std::size_t get_line() const noexcept { return line_; }
  std::size_t get_pos() const noexcept { return pos_; }
  std::size_t get_index() const noexcept { return index_; }

  char Cp() const noexcept;
  bool IsEnd() const noexcept;
  bool IsWhitespace() const noexcept;
  bool IsNewLine() const noexcept;
  void Next() noexcept;

  Position operator++(int) noexcept;
  Position operator++() noexcept;

  void Dump(std::ostream& os) const;

 private:
  std::shared_ptr<const std::string> text_;
  std::size_t line_;
  std::size_t pos_;
  std::size_t index_;
};

std::ostream& operator<<(std::ostream& os, const Position& position);

}  // namespace lexer

namespace std {

template <>
struct less<lexer::Position> {
  bool operator()(const lexer::Position& lhs,
                  const lexer::Position& rhs) const noexcept {
    return lhs.get_index() < rhs.get_index();
  }
};

}  // namespace std
