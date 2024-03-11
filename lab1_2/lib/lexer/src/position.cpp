#include "position.hpp"

namespace lexer {

bool Position::IsEnd() const noexcept { return (it_ == text_->end()); }

bool Position::IsWhitespace() const noexcept { return std::isspace(*it_); }

bool Position::IsNewLine() const noexcept {
  if (*it_ == '\r') {
    const auto suc = it_ + 1;
    return (suc != text_->end() && *suc == '\n');
  }

  return (*it_ == '\n');
}

void Position::Next() noexcept {
  if (IsNewLine()) {
    if (*it_ == '\r') {
      it_++;
    }

    line_++;
    pos_ = 1;
  } else {
    pos_++;
  }

  it_++;
}

void Position::Dump(std::ostream& os) const {
  os << "(" << line_ << ", " << pos_ << ")";
}

std::ostream& operator<<(std::ostream& os, const Position& p) {
  p.Dump(os);
  return os;
}

}  // namespace lexer
