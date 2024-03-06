#ifndef __CLI_UTILS_INL__
#define __CLI_UTILS_INL__

#include <cxxabi.h>

#include <algorithm>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

#include "cli/cli.h"

namespace cli {

template <typename T>
inline char const* QualifiedName() {
  static auto const name = abi::__cxa_demangle(typeid(std::decay_t<T>).name(),
                                               nullptr, nullptr, nullptr);
  return name;
}

inline std::string Camelify(std::string name) {
  name[0] = std::toupper(name[0]);
  return name;
}

inline std::string SplitCamel(std::string name) {
  std::vector<std::string> words;
  auto start = 0;
  for (auto i = 1UL; i < name.size(); ++i) {
    if (std::isupper(name[i])) {
      words.push_back(name.substr(start, i - start));
      start = i;
    }
  }

  words.push_back(name.substr(start));
  return std::accumulate(
      words.begin(), words.end(), std::string{},
      [](std::string const& a, std::string const& b) { return a + " " + b; });
}

inline std::string Spaces(size_t n) {
  return std::string(n, ' ');
}

inline std::string Align(std::string const& doc, size_t start) {
  std::string result;
  auto spaces = Spaces(start);
  for(auto c : doc) {
    result.push_back(c);
    if (c == '\n') {
      result += spaces;
    }
  }
  return result;
}

}  // namespace cli

#endif