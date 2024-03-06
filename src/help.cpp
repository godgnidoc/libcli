#include "cli/help.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <numeric>

namespace cli {

namespace {

template <typename T>
std::string Join(T const& set) {
  std::vector<std::string> list;
  for (auto const& item : set) {
    list.push_back(item);
  }

  std::sort(list.begin(), list.end(), [](auto const& a, auto const& b) {
    return a.size() < b.size() || (a.size() == b.size() && a < b);
  });

  return std::accumulate(list.begin(), list.end(), std::string{},
                         [](std::string a, std::string b) {
                           return a.empty() ? b : (a + ", " + b);
                         });
}

}  // namespace

DefaultHelp::DefaultHelp(cmd::settings const& settings)
    : Command({cmd::brief("Show help information"),
               cmd::args("[domain]... [command]")}) {
  for (auto setting : settings) setting(this);
}

int DefaultHelp::Execute(std::list<std::string> const& args) {
  if (args.empty()) return ShowGlobalHelp(), 0;

  std::map<std::string, Domain*> domain_map;
  for (auto domain : App::Inst().domains_) {
    domain_map[domain->name_] = domain;
  }

  std::map<std::string, Command*> command_map;
  for (auto command : App::Inst().commands_) {
    for (auto keyword : command->keywords_) {
      command_map[keyword] = command;
    }
  }
  Command* command = nullptr;
  Domain* domain = nullptr;

  auto it = args.begin();
  while (it != args.end()) {
    auto cit = command_map.find(*it);
    if (cit != command_map.end()) {
      ++it;
      command = cit->second;
      break;
    }

    auto dit = domain_map.find(*it);
    if (dit == domain_map.end()) {
      std::cerr << "Unknown domain or command: ";
      for (auto arg : args) std::cerr << arg << " ";
      std::cerr << std::endl;
      return 1;
    }

    ++it;
    domain = dit->second;
    command_map.clear();
    for (auto command : domain->commands_) {
      for (auto keyword : command->keywords_) {
        command_map[keyword] = command;
      }
    }
    domain_map.clear();
    for (auto subdomain : domain->domains_) {
      domain_map[subdomain->name_] = subdomain;
    }
  }

  if (it != args.end()) {
    std::cerr << "junk at the end of command: ";
    while (it != args.end()) std::cerr << *it++ << " ";
    std::cerr << std::endl;
    return 1;
  }

  if (command) return ShowCommandHelp(command, args), 0;
  return ShowDomainHelp(domain, args), 0;
}

void DefaultHelp::ShowCommandHelp(Command* command,
                                  std::list<std::string> const& args) const {
  std::cout << "Usage: " << App::Inst().name_ << " [global-options]... ";
  for (auto arg : args) std::cout << arg << " ";
  if (!command->options_.empty()) std::cout << "[options]... ";
  if (command->args_) std::cout << *command->args_;
  std::cout << std::endl;

  if (command->brief_) {
    std::cout << std::endl << "  " << *command->brief_ << std::endl;
  }

  if (!command->options_.empty()) {
    std::cout << std::endl << "Options:";

    for (auto option : command->options_) {
      std::cout << std::endl << "  " << Join(option->keywords_);
      if (option->brief_) {
        std::cout << "    --  " << *option->brief_;
      }
    }
  }

  auto& app = App::Inst();
  if (!app.options_.empty()) {
    std::cout << std::endl << "Global Options:";

    for (auto option : app.options_) {
      std::cout << std::endl << "  " << Join(option->keywords_);
      if (option->brief_) {
        std::cout << "    --  " << *option->brief_;
      }
    }
  }

  std::cout << std::endl;
}

void DefaultHelp::ShowDomainHelp(Domain* domain,
                                 std::list<std::string> const& args) const {
  (void)args;

  if (!domain->commands_.empty()) {
    std::cout << std::endl << "Commands:";

    for (auto command : domain->commands_) {
      std::cout << std::endl << "  " << Join(command->keywords_);
      if (command->brief_) {
        std::cout << "    --  " << *command->brief_;
      }
    }
  }

  if (!domain->domains_.empty()) {
    std::cout << std::endl << "Domains:";

    for (auto domain : domain->domains_) {
      std::cout << std::endl << "  " << domain->name_;
    }
  }

  std::cout << std::endl;
}

void DefaultHelp::ShowGlobalHelp() const {
  auto& app = App::Inst();

  std::cout << app.name_
            << " [global-options]... [domain]..."
               " <command> [options]... [arguments]..."
            << std::endl;

  if (app.brief_) {
    std::cout << std::endl << "  " << *app.brief_ << std::endl;
  }

  if (!app.options_.empty()) {
    std::cout << std::endl << "Global Options:";

    for (auto option : app.options_) {
      std::cout << std::endl << "  " << Join(option->keywords_);
      if (option->brief_) {
        std::cout << "    --  " << *option->brief_;
      }
    }
  }

  if (!app.commands_.empty()) {
    std::cout << std::endl << "Commands:";

    for (auto command : app.commands_) {
      std::cout << std::endl << "  " << Join(command->keywords_);
      if (command->brief_) {
        std::cout << "    --  " << *command->brief_;
      }
    }
  }

  if (!app.domains_.empty()) {
    std::cout << std::endl << "Domains:";

    for (auto domain : app.domains_) {
      std::cout << std::endl << "  " << domain->name_;
    }
  }

  std::cout << std::endl;
}

}  // namespace cli