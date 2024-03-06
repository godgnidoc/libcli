#ifndef __CLI_ARG_H__
#define __CLI_ARG_H__

#include <format>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cli/cli.h"

namespace cli {

class ArgDef {
  friend class App;

 public:
  ArgDef(std::string const& name);
  ArgDef(ArgDef const&) = delete;
  ArgDef(ArgDef&&) = delete;
  ArgDef& operator=(ArgDef const&) = delete;
  ArgDef& operator=(ArgDef&&) = delete;
  ~ArgDef() = default;

  bool HasValue() const;

  std::string Value() const;

  std::vector<std::string> Values() const;

 private:
  std::string name_;
  std::vector<std::string> values_;
};

Arg Argument(std::string name);
Arg Optional(std::string name);
Arg Some(std::string name);
Arg More();

}  // namespace cli

#endif