#include "cli/domain.h"

#include <map>

#include "cli/app.h"
#include "cli/command.h"

namespace cli {

namespace domain {

setting bind(Domain* d) {
  return [d](Domain* domain) { domain->AddDomain(d); };
}

setting name(std::string const& n) {
  return [n](Domain* domain) { domain->name_ = n; };
}

}  // namespace domain

Domain::Domain(domain::settings const& settings) {
  for (auto const& setting : settings) {
    setting(this);
  }
}

void Domain::AddCommand(Command* command) { commands_.insert(command); }

void Domain::AddDomain(Domain* domain) { domains_.insert(domain); }

Command* Domain::LocateEntry(std::list<std::string>& args) {
  return LocateEntry(args, args.begin());
}

Command* Domain::LocateEntry(std::list<std::string>& args,
                             typename std::list<std::string>::iterator it) {
  std::map<std::string, Command*> command_map;
  for (auto command : commands_) {
    for (auto keyword : command->keywords_) {
      command_map[keyword] = command;
    }
  }

  std::map<std::string, Domain*> domain_map;
  for (auto domain : domains_) {
    domain_map[domain->name_] = domain;
  }

  auto eoo = App::Inst().end_of_options_;
  while (it != args.end() && *it != eoo) {
    auto cit = command_map.find(*it);
    if (cit != command_map.end()) {
      args.erase(it);
      return cit->second;
    }

    auto dit = domain_map.find(*it);
    if (dit == domain_map.end()) {
      ++it;
      continue;
    }

    auto domain = dit->second;
    auto command = domain->LocateEntry(args, std::next(it));
    if (!command) continue;

    args.erase(it);
    return command;
  }

  return nullptr;
}

}  // namespace cli