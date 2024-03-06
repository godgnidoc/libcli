#ifndef __CLI_OPT_INL__
#define __CLI_OPT_INL__

#include <iostream>

#include "cli/cli.h"

namespace cli {

inline OptDef::OptDef(std::vector<std::string> const& flags) : flags_{flags} {}

inline bool OptDef::IsOn() const { return !repeats_.empty(); }

inline std::vector<std::string> OptDef::AllArgs() const {
  std::vector<std::string> result;
  for (auto const& args : repeats_) {
    for (auto const& arg : args) {
      result.push_back(arg);
    }
  }
  return result;
}

inline size_t OptDef::Times() const { return repeats_.size(); }

inline std::vector<std::string> OptDef::ArgsAt(size_t index) const {
  return repeats_.at(index);
}

inline Opt OptDef::Doc(std::string const& doc) {
  doc_ = doc;
  return shared_from_this();
}

inline Opt OptDef::Required() {
  required_ = true;
  return shared_from_this();
}

inline Opt OptDef::Repeatable() {
  repeatable_ = true;
  return shared_from_this();
}

inline Opt OptDef::Arg(std::string const& name) {
  args_.push_back(Argument(name));
  return shared_from_this();
}

inline Opt OptDef::Global() {
  App::AddOption(shared_from_this());
  return shared_from_this();
}

template <typename... Args>
inline Opt Option(std::string const& flag, Args const&... more_flags) {
  auto opt = Opt{new OptDef{{flag, more_flags...}}};
  return opt;
}

}  // namespace cli

#endif