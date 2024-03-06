#ifndef __CLI_FN_INL__
#define __CLI_FN_INL__

#include "cli/cli.h"

namespace cli {

inline FnCommand::FnCommand(std::function<Fn> const& fn) : fn_{fn} {}

inline int FnCommand::Run(std::vector<std::string> args) {
  return fn_(args);
}

inline FnCommand* FnCommand::Doc(std::string const& doc) {
  CmdDef::Doc(doc);
  return this;
}

inline FnCommand* FnCommand::Brief(std::string const& brief) {
  CmdDef::Brief(brief);
  return this;
}

inline FnCommand* FnCommand::Use(Opt opt) {
  for (auto const& flag : App::FlagsOf(opt)) {
    opts_[flag] = opt;
  }
  return this;
}

inline FnCommand* FnCommand::Arg(std::string const& name) {
  CmdDef::Argument(name);
  return this;
}

inline FnCommand* FnCommand::Optional(std::string const& name) {
  CmdDef::Optional(name);
  return this;
}

inline FnCommand* FnCommand::Some(std::string const& name) {
  CmdDef::Some(name);
  return this;
}

inline FnCommand* FnCommand::More() {
  CmdDef::More();
  return this;
}


inline FnCommand* CmdFn(std::string const& name, std::function<Fn> const& fn) {
  auto cmd = new FnCommand{fn};
  App::AddCmd(name, Ctx{cmd});
  return cmd;
}

inline FnCommand* MainFn(std::string const& name, std::function<Fn> const& fn) {
  auto cmd = new FnCommand{fn};
  App::AddCmd(name, Ctx{cmd});
  App::SetMain(name);
  return cmd;
}

}  // namespace cli

#endif