#ifndef __CLI_VERSION_H__
#define __CLI_VERSION_H__

#include "cli/app.h"
#include "cli/command.h"

namespace cli {
class DefaultVersion : public Command {
 public:
  DefaultVersion(cmd::settings const& settings);

  int Execute(std::list<std::string> const& args) override;
};
}  // namespace cli

#endif