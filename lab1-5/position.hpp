#pragma once

#include <memory>
#include <string>

namespace lexer {

constexpr unsigned char kEofCharacter = -1;

class Position final {
 public:
  Position(std::shared_ptr<const std::string> text) noexcept
      : text_(std::move(text)) {}

  std::size_t get_line() const noexcept { return line_; }
  std::size_t get_pos() const noexcept { return pos_; }
  std::size_t get_index() const noexcept { return index_; }

  char Cp() const noexcept;
  bool IsWhitespace() const noexcept;
  bool IsNewLine() const noexcept;
  void Next() noexcept;

  Position operator++(int) noexcept;
  Position operator++() noexcept;

  void Dump(std::ostream& os) const;

 private:
  std::shared_ptr<const std::string> text_ = nullptr;
  std::size_t line_ = 1;
  std::size_t pos_ = 1;
  std::size_t index_ = 0;
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
