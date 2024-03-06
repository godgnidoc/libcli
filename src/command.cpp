#include "cli/command.h"

#include <stdexcept>

#include "cli/app.h"
#include "cli/domain.h"

namespace cli {

namespace cmd {
setting global() { return bind(&App::Inst()); }

setting bind(Domain* d) {
  return [d](Command* c) { d->AddCommand(c); };
}

setting brief(std::string const& b) {
  return [b](Command* c) { c->brief_ = b; };
}

setting keyword(std::string const& k) {
  return [k](Command* c) { c->keywords_.insert(k); };
}

setting args(std::string const& a) {
  return [a](Command* c) { c->args_ = a; };
}

setting defaults() {
  return [](Command* c) { App::Inst().SetDefaultCommand(c); };
};

}  // namespace cmd

Command::Command(cmd::settings const& settings) {
  for (auto const& setting : settings) setting(this);
}

}  // namespace cli