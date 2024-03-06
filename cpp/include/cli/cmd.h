#ifndef __CLI_CMD_H__
#define __CLI_CMD_H__

#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include "cli/cli.h"

namespace cli {

class CmdDef : public std::enable_shared_from_this<CmdDef> {
  friend class App;

 public:
  CmdDef() = default;
  CmdDef(CmdDef const&) = delete;
  CmdDef(CmdDef&&) = delete;
  CmdDef& operator=(CmdDef const&) = delete;
  CmdDef& operator=(CmdDef&&) = delete;
  virtual ~CmdDef() = default;

  virtual int Run(std::vector<std::string> args) = 0;

 protected:
  template <typename... Args>
  Opt Option(std::string const& flag, Args const&... more_flags);

  Arg Argument(std::string const& name);
  Arg Optional(std::string const& name);
  Arg Some(std::string const& name);
  Arg More();

  void Doc(std::string const& doc);
  void Brief(std::string const& brief);

 protected:
  std::optional<std::string> doc_;
  std::optional<std::string> brief_;
  std::map<std::string, Opt> opts_;
  std::vector<Arg> args_;
};

template <typename T, typename... Args>
void Cmd(Args&&... args);

template <typename T, typename... Args>
void Main(Args&&... args);

}  // namespace cli

#endif