#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace fmt {

class IdentTable final {
  std::unordered_map<std::string, std::size_t> codes_;
  std::vector<std::string> names_;

 public:
  std::size_t GetCode(const std::string& name);
  const std::string& At(const std::size_t code) const;
};

}  // namespace fmt
