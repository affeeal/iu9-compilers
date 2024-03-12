#pragma once

#include <memory>

namespace lexer {

class Position final {
 public:
  Position(std::shared_ptr<const std::string> text) noexcept
      : text_(std::move(text)), it_(text_->cbegin()), line_(1), pos_(1) {}

  std::string::const_iterator get_it() const noexcept { return it_; }
  std::size_t get_line() const noexcept { return line_; }
  std::size_t get_pos() const noexcept { return pos_; }

  bool IsEnd() const noexcept;
  bool IsWhitespace() const noexcept;
  bool IsNewLine() const noexcept;
  void Next() noexcept;

  void Dump(std::ostream& os) const;

 private:
  std::shared_ptr<const std::string> text_;
  std::string::const_iterator it_;
  std::size_t line_;
  std::size_t pos_;
};

std::ostream& operator<<(std::ostream& os, const Position& p);

}  // namespace lexer

namespace std {

template <>
struct less<lexer::Position> {
  bool operator()(const lexer::Position& lhs,
                  const lexer::Position& rhs) const noexcept {
    return (lhs.get_line() < rhs.get_line() ||
            lhs.get_line() == rhs.get_line() && lhs.get_pos() < rhs.get_pos());
  }
};

}  // namespace std
