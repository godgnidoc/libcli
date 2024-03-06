#include "cli/option.h"

#include <map>
#include <stdexcept>

#include "cli/app.h"
#include "cli/command.h"
#include "cli/with_options.h"

namespace cli {

namespace opt {

setting brief(std::string const& b) {
  return [b](Option* o) { o->brief_ = b; };
}

setting required() {
  return [](Option* o) { o->required_ = true; };
};

setting repeatable() {
  return [](Option* o) { o->repeatable_ = true; };
};

setting keyword(std::string const& k) {
  return [k](Option* o) { o->keywords_.insert(k); };
}

setting arg(std::set<std::string> const& a) {
  return [a](Option* o) {
    o->arg_ = [a](std::string const& v) {
      if (a.find(v) == a.end()) {
        throw std::invalid_argument{"Invalid argument: " + v};
      }
    };
  };
}

setting arg(arg_validator a) {
  return [a](Option* o) { o->arg_ = a; };
}

setting arg() {
  return [](Option* o) { o->arg_ = [](std::string const&) {}; };
}

setting bind(WithOptions* o) {
  return [o](Option* opt) { o->AddOption(opt); };
}

setting global() { return bind(&App::Inst()); };

}  // namespace opt

Option::Option(opt::settings const& settings) {
  for (auto const& setting : settings) setting(this);
}

Option::operator bool() const { return !values_.empty(); }
size_t Option::size() const { return values_.size(); }

std::string const& Option::operator[](std::size_t i) const {
  return values_[i];
}

typename std::vector<std::string>::const_iterator Option::begin() const {
  return values_.begin();
}

typename std::vector<std::string>::const_iterator Option::end() const {
  return values_.end();
}

std::string const& Option::operator*() const {
  if (values_.empty()) throw std::runtime_error{"Option has no value"};
  return values_.front();
}

std::string const* Option::operator->() const {
  if (values_.empty()) throw std::runtime_error{"Option has no value"};
  return &values_.front();
}

}  // namespace cli