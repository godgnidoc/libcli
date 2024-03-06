#ifndef __CLI_ARG_INL__
#define __CLI_ARG_INL__

#include "cli/cli.h"

namespace cli {

inline ArgDef::ArgDef(std::string const& name) : name_{name} {}

inline bool ArgDef::HasValue() const { return !values_.empty(); }

inline std::string ArgDef::Value() const {
  std::string result;
  for (auto const& value : values_) {
    if (!result.empty()) {
      result += " ";
    }
    result += value;
  }
  return result;
}

inline std::vector<std::string> ArgDef::Values() const { return values_; }

inline Arg Argument(std::string name) {
  name.insert(0, "<");
  name.append(">");
  return Arg{new ArgDef{name}};
}
inline Arg Optional(std::string name) {
  name.insert(0, "[");
  name.append("]");
  return Arg{new ArgDef{name}};
}

inline Arg Some(std::string name) {
  name.insert(0, "<");
  name.append(">...");
  return Arg{new ArgDef{name}};
}

inline Arg More() { return Arg{new ArgDef{"..."}}; }

}  // namespace cli

#endif