#include <iostream>

#include <boost/regex.hpp>

void TestBoostRegexNamedCaptureGroup() {
  std::string str("abc123");
  boost::regex reg("(?<letters>[a-z]+)([0-9]+)");
  boost::smatch res;

  boost::regex_match(str, res, reg);
  std::cout << res["letters"].str() << std::endl;
}

int main() {
  TestBoostRegexNamedCaptureGroup();
}
