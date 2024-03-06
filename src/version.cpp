#include "cli/version.h"

#include <iostream>

namespace cli {

DefaultVersion::DefaultVersion(cmd::settings const& settings)
    : Command({cmd::brief("Show version information")}) {
  for (auto setting : settings) setting(this);
}

int DefaultVersion::Execute(std::list<std::string> const& args) {
  (void)args;
  auto& app = App::Inst();
  std::cout << app.name_;
  if (app.version_) std::cout << " " << *app.version_;
  std::cout << std::endl;
  if (app.brief_) std::cout << *app.brief_ << std::endl;
  return 0;
}

}  // namespace cli