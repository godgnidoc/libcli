#ifndef __CLI_FN_H__
#define __CLI_FN_H__

#include <functional>

#include "cli/cli.h"

namespace cli {

class FnCommand : public CmdDef {
 public:
  FnCommand(std::function<Fn> const& fn);

  int Run(std::vector<std::string> args) override;

  FnCommand* Doc(std::string const& doc);

  FnCommand* Brief(std::string const& brief);

  FnCommand* Use(Opt opt);

  FnCommand* Arg(std::string const& name);

  FnCommand* Optional(std::string const& name);

  FnCommand* Some(std::string const& name);

  FnCommand* More();

 private:
  std::function<Fn> fn_;
};

FnCommand* CmdFn(std::string const& name, std::function<Fn> const& fn);
FnCommand* MainFn(std::string const& name, std::function<Fn> const& fn);

}  // namespace cli

#endif