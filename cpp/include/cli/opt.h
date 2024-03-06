#ifndef __CLI_OPT_H__
#define __CLI_OPT_H__

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "cli/cli.h"

namespace cli {

struct OptDef : public std::enable_shared_from_this<OptDef> {
  friend class App;

 public:
  OptDef(std::vector<std::string> const& flags);
  OptDef(OptDef const&) = delete;
  OptDef(OptDef&&) = delete;
  OptDef& operator=(OptDef const&) = delete;
  OptDef& operator=(OptDef&&) = delete;
  ~OptDef() = default;

  bool IsOn() const;

  std::vector<std::string> AllArgs() const;

  size_t Times() const;

  std::vector<std::string> ArgsAt(size_t index) const;

  Opt Doc(std::string const& doc);

  Opt Required();

  Opt Repeatable();

  Opt Arg(std::string const& name);

  Opt Global();

 private:
  std::optional<std::string> doc_;
  std::vector<std::string> flags_;
  std::vector<cli::Arg> args_;
  bool required_{false};
  bool repeatable_{false};

  std::vector<std::vector<std::string>> repeats_;
};

template <typename... Args>
Opt Option(std::string const& flag, Args const&... more_flags);

}  // namespace cli

#endif