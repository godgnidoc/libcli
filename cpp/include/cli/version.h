#ifndef __CLI_VERSION_H__
#define __CLI_VERSION_H__

#include "cli/cli.h"

namespace cli {

class Version : public CmdDef {
 public:
  Version() { Doc("Show version information."); }

  int Run(std::vector<std::string>) override {
    if (verbose_->IsOn()) {
      if (auto name = App::Name(); name) {
        std::cout << *name << " - ";
      }

      std::cout << App::Version().value_or("0.0.0");

      if (auto author = App::Author(); author) {
        std::cout << " - " << *author;
      }

      std::cout << std::endl;
    }
    return 0;
  }

 private:
  Opt verbose_ = Option("-v", "--verbose")->Doc("Show verbose output.");
};

}  // namespace cli

#endif