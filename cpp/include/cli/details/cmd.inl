#ifndef __CLI_CMD_INL__
#define __CLI_CMD_INL__

#include "cli/cli.h"
#include "cli/details/utils.inl"

namespace cli {

template <typename... Args>
inline Opt CmdDef::Option(std::string const& flag, Args const&... more_flags) {
  auto opt = cli::Option(flag, more_flags...);
  for (auto const& flag : App::FlagsOf(opt)) {
    opts_[flag] = opt;
  }
  return opt;
}

inline Arg CmdDef::Argument(std::string const& name) {
  auto arg = cli::Argument(name);
  args_.push_back(arg);
  return arg;
}
inline Arg CmdDef::Optional(std::string const& name) {
  auto arg = cli::Optional(name);
  args_.push_back(arg);
  return arg;
}
inline Arg CmdDef::Some(std::string const& name) {
  auto arg = cli::Some(name);
  args_.push_back(arg);
  return arg;
}
inline Arg CmdDef::More() {
  auto arg = cli::More();
  args_.push_back(arg);
  return arg;
}

inline void CmdDef::Doc(std::string const& doc) { doc_ = doc; }
inline void CmdDef::Brief(std::string const& brief) { brief_ = brief; }

template <typename T, typename... Args>
inline void Cmd(Args&&... args) {
  static_assert(std::is_base_of_v<CmdDef, T>, "T must extends CmdDef");
  auto name = Camelify(QualifiedName<T>());
  auto scope = name.find_last_of(':');
  if (scope != std::string::npos) {
    name = name.substr(scope + 1);
  }
  App::AddCmd(name, std::make_shared<T>(std::forward<Args>(args)...));
} 

template <typename T, typename... Args>
inline void Main(Args&&... args) {
  static_assert(std::is_base_of_v<CmdDef, T>, "T must extends CmdDef");
  auto name = Camelify(QualifiedName<T>());
  auto scope = name.find_last_of(':');
  if (scope != std::string::npos) {
    name = name.substr(scope + 1);
  }
  App::AddCmd(name, std::make_shared<T>(std::forward<Args>(args)...));
  App::SetMain(name);
}

}  // namespace cli

#endif