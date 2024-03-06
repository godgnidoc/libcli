#include "cli/app.h"

#include <filesystem>
#include <iostream>

#include "cli/command.h"

namespace cli {

App::App()
    : name_{std::filesystem::read_symlink("/proc/self/exe")
                .filename()
                .string()},
      default_command_{nullptr} {}

App& App::Inst() {
  static App app;
  return app;
}

int App::Execute(int argc, char** argv, std::function<int()> setup) {
  std::list<std::string> args;
  for (int i = 1; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }
  return Inst().Execute(args, setup);
}

void App::AddGlobalCommand(Command* command) { Inst().AddCommand(command); }

void App::AddGlobalDomain(Domain* domain) { Inst().AddDomain(domain); }

void App::AddGlobalOption(Option* option) { Inst().AddOption(option); }

void App::SetEndOfOptions(std::string const& end_of_options) {
  Inst().end_of_options_ = end_of_options;
}

void App::SetDefaultCommand(Command* command) {
  Inst().default_command_ = command;
}

void App::SetName(std::string const& name) { Inst().name_ = name; }

void App::SetBrief(std::string const& brief) { Inst().brief_ = brief; }

void App::SetVersion(std::string const& version) { Inst().version_ = version; }

int App::Execute(std::list<std::string>& args,
                 std::function<int()> const& setup) {
  /**
   * 分析全局选项
   */
  try {
    ParseOptions(args);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  /**
   * 分析命令
   */
  auto command = LocateEntry(args);
  if (!command) command = default_command_;
  if (!command) {
    std::cerr << "error: Unknown command" << std::endl;
    return 1;
  }

  /**
   * 分析指令选项
   */
  try {
    command->ParseOptions(args);
  } catch (std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }

  /**
   * 删除选项结束符
   */
  for (auto it = args.begin(); it != args.end(); ++it) {
    if (*it == end_of_options_) {
      args.erase(it);
      break;
    }
  }

  /**
   * 检查参数
   */
  if (!command->args_ && !args.empty()) {
    std::cerr << "error: Command does not take arguments" << std::endl;
    return 1;
  }

  /**
   * 执行环境预处理
   */
  if (setup) {
    auto ret = setup();
    if (ret != 0) return ret;
  }

  /**
   * 执行命令
   */
  return command->Execute(args);
}

}  // namespace cli