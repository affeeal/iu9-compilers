#include "position.hpp"

namespace lexer {

char Position::Cp() const noexcept {
  return (index_ == text_->size() ? kEnd : text_->at(index_));
}

bool Position::IsEnd() const noexcept { return index_ == text_->size(); }

bool Position::IsWhitespace() const noexcept {
  return (index_ != text_->size() && std::isspace(text_->at(index_)));
}

bool Position::IsNewLine() const noexcept {
  if (index_ == text_->size()) {
    return false;
  }

  if (text_->at(index_) == '\r' && index_ + 1 < text_->size()) {
    return (text_->at(index_ + 1) == '\n');
  }

  return (text_->at(index_) == '\n');
}

void Position::Next() noexcept {
  if (index_ == text_->size()) {
    return;
  }

  if (IsNewLine()) {
    if (text_->at(index_) == '\r') {
      ++index_;
    }

    ++line_;
    pos_ = 1;
  } else {
    ++pos_;
  }

  ++index_;
}

Position Position::operator++(int) noexcept {
  auto old = *this;
  Next();
  return old;
}

Position Position::operator++() noexcept {
  Next();
  return *this;
}

void Position::Dump(std::ostream& os) const {
  os << "(" << line_ << ", " << pos_ << ")";
}

std::ostream& operator<<(std::ostream& os, const Position& position) {
  position.Dump(os);
  return os;
}

}  // namespace lexer
