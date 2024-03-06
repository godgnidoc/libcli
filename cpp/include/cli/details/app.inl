#ifndef __CLI_APP_INL__
#define __CLI_APP_INL__

#include <iostream>

#include "cli/cli.h"
#include "cli/details/utils.inl"

namespace cli {

inline App& App::Inst() {
  static App inst;
  return inst;
}

inline void App::SetName(std::string const& name) { Inst().name_ = name; }
inline void App::SetUsage(std::string const& usage) { Inst().usage_ = usage; }
inline void App::SetVersion(std::string const& version) {
  Inst().version_ = version;
}
inline void App::SetAuthor(std::string const& author) {
  Inst().author_ = author;
}

inline void App::SetMain(std::string const& name) { Inst().main_ = name; }
inline void App::AddCmd(std::string const& name, Ctx cmd) {
  Inst().commands_[name] = cmd;
}
inline void App::AddOption(Opt opt) {
  for (auto const& flag : opt->flags_) {
    Inst().options_[flag] = opt;
  }
}

inline std::map<std::string, Opt> App::Options() { return Inst().options_; }
inline std::map<std::string, Ctx> App::Commands() { return Inst().commands_; }
inline std::optional<std::string> App::Name() { return Inst().name_; }
inline std::optional<std::string> App::Usage() { return Inst().usage_; }
inline std::optional<std::string> App::Version() { return Inst().version_; }
inline std::optional<std::string> App::Author() { return Inst().author_; }
inline std::optional<std::string> App::Main() { return Inst().main_; }

inline std::optional<std::string> App::DocOf(Ctx cmd) { return cmd->doc_; }
inline std::optional<std::string> App::BriefOf(Ctx cmd) { return cmd->brief_; }
inline std::map<std::string, Opt> App::OptionsOf(Ctx cmd) { return cmd->opts_; }
inline std::vector<Arg> App::ArgsOf(Ctx cmd) { return cmd->args_; }

inline std::optional<std::string> App::DocOf(Opt opt) { return opt->doc_; }
inline std::vector<std::string> App::FlagsOf(Opt opt) { return opt->flags_; }
inline std::vector<Arg> App::ArgsOf(Opt opt) { return opt->args_; }
inline bool App::IsRequired(Opt opt) { return opt->required_; }
inline bool App::IsRepeatable(Opt opt) { return opt->repeatable_; }

inline std::string App::NameOf(Arg arg) { return arg->name_; }

inline bool App::IsOptional(Arg arg) {
  return arg->name_ == "..." ||
         (arg->name_.front() == '[' && arg->name_.back() == ']');
}

inline int App::Run(std::vector<std::string> args) {
  // 分割短选项
  SplitShorts(args);

  // 扫描并消费全局选项
  if (!ConsumeOptions(App::Options(), args)) {
    return -1;
  }

  // 查找并确定要执行的命令
  auto selection = ConsumeKeywords(args);
  if (selection.empty()) {
    std::cerr << "Command not found" << std::endl;
    return -1;
  } else if (selection.size() > 1) {
    std::cerr << "Ambiguous command" << std::endl;
    return -1;
  }

  auto cmd = Inst().commands_[*selection.begin()];
  if (cmd == nullptr) {
    std::cerr << "Command not found" << std::endl;
    return -1;
  }

  // 扫描并消费命令选项
  if (!ConsumeOptions(cmd->opts_, args)) {
    return -1;
  }

  auto fn = std::dynamic_pointer_cast<FnCommand>(cmd);

  // 消费具名参数
  if (!ConsumeArguments(cmd->args_, args, !fn)) {
    return -1;
  }

  return cmd->Run(args);
}

inline std::set<std::string> App::ConsumeKeywords(
    std::vector<std::string>& args) {
  auto& inst = Inst();
  std::vector<std::string> rest;
  std::set<std::string> selection;

  for (auto const& it : inst.commands_) {
    selection.insert(it.first);
  }

  std::string accept = "";
  for (size_t i = 0; i < args.size(); i++) {
    auto test = accept + Camelify(args[i]);
    std::set<std::string> narrow;
    for (auto const& sel : selection) {
      if (sel.find(test) == 0) {
        narrow.insert(sel);
      }
    }

    if (narrow.empty()) {
      rest.push_back(args[i]);
      continue;
    }

    selection = narrow;
    accept = test;
  }

  if (selection.size() > 1 && selection.size() == inst.commands_.size() &&
      inst.main_.has_value()) {
    selection = {inst.main_.value()};
  }

  args = rest;
  return selection;
}

inline bool App::ConsumeOptions(std::map<std::string, Opt> const& options,
                                std::vector<std::string>& args) {
  std::vector<std::string> rest;
  for (size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "--") {
      rest.insert(rest.end(), args.begin() + i, args.end());
      break;
    }

    auto opit = options.find(args[i]);
    if (opit == options.end()) {
      rest.push_back(args[i]);
      continue;
    }

    auto opt = opit->second;
    opt->repeats_.push_back({});
    auto& orgs = opt->repeats_.back();

    for (size_t j = 0; j < opt->args_.size(); ++j) {
      if (++i >= args.size()) {
        return false;
      }
      orgs.push_back(args[i]);
    }
  }

  for (auto const& [flag, opt] : options) {
    if (opt->required_ && !opt->IsOn()) {
      std::cerr << "option " << flag << " is required" << std::endl;
      return false;
    }

    if (!opt->repeatable_ && opt->Times() > 1) {
      std::cerr << "option " << flag << " can only be used once" << std::endl;
      return false;
    }
  }

  args = rest;
  return true;
}

inline bool App::ConsumeArguments(std::vector<Arg> crgs,
                                  std::vector<std::string>& args, bool erase) {
  ClearArguments(args);

  while (!crgs.empty()) {
    auto crg = crgs.front();
    if (crg->name_ == "...") {
      crg->values_ = args;
      break;
    }

    if (args.empty()) {
      if (!IsOptional(crg)) {
        std::cerr << "argument " << crg->name_ << " is required" << std::endl;
        return false;
      }
      break;
    }

    if (crg->name_.find("...") == crg->name_.size() - 3) {
      crg->values_ = args;
      break;
    }

    crg->values_ = {args.front()};
    crgs.erase(crgs.begin());

    if (!erase) {
      continue;
    }

    args.erase(args.begin());
  }

  return true;
}

inline void App::SplitShorts(std::vector<std::string>& args) {
  std::vector<std::string> rest;
  for (size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "--") {
      rest.insert(rest.end(), args.begin() + i, args.end());
      break;
    }

    if (args[i].length() <= 2 || args[i][0] != '-' || args[i][1] == '-') {
      rest.push_back(args[i]);
      continue;
    }

    for (size_t j = 1; j < args[i].length(); ++j) {
      rest.push_back("-" + std::string(1, args[i][j]));
    }
  }
  args = rest;
}

inline void App::ClearArguments(std::vector<std::string>& args) {
  std::vector<std::string> rest;
  for (size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "--") {
      rest.insert(rest.end(), args.begin() + i, args.end());
      break;
    }

    rest.push_back(args[i]);
  }
  args = rest;
}

inline int Run(int argc, char** argv) {
  std::vector<std::string> args{argv + 1, argv + argc};
  return App::Run(args);
}

}  // namespace cli

#endif