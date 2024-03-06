#ifndef __CLI_HELP_H__
#define __CLI_HELP_H__

#include <list>

#include "cli/app.h"
#include "cli/command.h"

namespace cli {

class DefaultHelp : public Command {
 public:
  DefaultHelp(cmd::settings const& settings);

  int Execute(std::list<std::string> const& args) override;

 private:
  void ShowCommandHelp(Command* command,
                       std::list<std::string> const& args) const;

  void ShowDomainHelp(Domain* domain, std::list<std::string> const& args) const;

  void ShowGlobalHelp() const;
};

}  // namespace cli

#endif