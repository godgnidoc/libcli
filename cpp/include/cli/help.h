#ifndef __CLI_HELP_H__
#define __CLI_HELP_H__

#include <algorithm>

#include "cli/cli.h"
#include "cli/details/utils.inl"

namespace cli {

class Help : public CmdDef {
 public:
  Help() {
    Brief("Show help");
    Doc("Show help, specify a command to show detailed help");
  }

  int Run(std::vector<std::string> args) override {
    if (!args.empty()) {
      auto selection = App::ConsumeKeywords(args);
      if (selection.size() != 1 || !args.empty()) {
        std::cerr << "Invalid command: " << args[0] << std::endl;
        return 1;
      }

      auto cmd = App::Commands().at(*selection.begin());
      DocumentCommand(cmd, false);
    } else {
      if (App::Main()) {
        auto main = App::Commands().at(App::Main().value());
        DocumentCommand(main, true);
      } else {
        std::cout << "Usage: " << App::Name().value_or("")
                  << " [global-options]"
                  << " <command>"
                  << " [command-options]"
                  << " [arguments]" << std::endl;
        std::cout << std::endl;
        std::cout << App::Usage().value_or("") << std::endl;
        std::cout << std::endl;
      }

      std::cout << "Commands:" << std::endl;
      BriefCommands();
    }

    if (!App::Options().empty()) {
      std::cout << "Global options:" << std::endl;
      BriefOptions(App::Options());
      std::cout << std::endl;
    }

    return 0;
  }

  void DocumentCommand(Ctx cmd, bool omit_main) {
    std::cout << "Usage: " << App::Name().value_or("") << " ";

    for (auto const& [key, value] : App::Commands()) {
      if (value != cmd) continue;

      if (key != App::Main().value_or("") || !omit_main) {
        std::cout << SplitCamel(key) << " ";
      }

      break;
    }

    auto options = App::OptionsOf(cmd);
    if (!options.empty()) {
      std::cout << " [command-options] ";
    }

    auto args = App::ArgsOf(cmd);
    for (auto const& arg : args) {
      std::cout << arg << " ";
    }

    std::cout << std::endl << std::endl;

    if (auto doc = App::DocOf(cmd); doc) {
      std::cout << *doc << std::endl << std::endl;
    } else if (auto brief = App::BriefOf(cmd); brief) {
      std::cout << *brief << std::endl << std::endl;
    }

    if (!options.empty()) {
      std::cout << "Options:" << std::endl;
      BriefOptions(options);
      std::cout << std::endl;
    }
  }

  void BriefCommands() {
    auto max_width = 0UL;
    for (auto const& [key, value] : App::Commands()) {
      max_width = std::max(max_width, key.size());
    }

    auto start = (max_width + 6 + 7) / 8 * 8;
    for (auto const& [name, cmd] : App::Commands()) {
      if (auto brief = App::BriefOf(cmd); brief) {
        std::cout << "  " << SplitCamel(name);
      } else {
        auto doc = "  " + SplitCamel(name);
        doc += Spaces(start - name.size()) + *brief;
        doc = Align(doc, start);
        std::cout << doc << std::endl;
      }
    }
  }

  void BriefOptions(std::map<std::string, Opt> const& opts) {
    std::map<Opt, std::string> sorted;
    auto max_width = 0UL;
    for (auto const& [flag, opt] : opts) {
      if (sorted.count(opt)) {
        sorted[opt] += ", " + flag;
      } else {
        sorted[opt] = flag;
      }
      max_width = std::max(max_width, sorted[opt].size());
    }

    auto start = (max_width + 4 + 7) / 8 * 8;
    for (auto const& [opt, flags] : sorted) {
      auto gap = start - flags.size();
      auto doc = flags + Spaces(gap) + App::DocOf(opt).value_or("");
      doc = Align(doc, start);
      std::cout << doc << std::endl;
    }
  }
};

}  // namespace cli

#endif