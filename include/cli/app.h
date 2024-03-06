#ifndef __CLI_APP_H__
#define __CLI_APP_H__

#include <list>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "cli/domain.h"
#include "cli/option.h"
#include "cli/with_options.h"

namespace cli {

class Command;

/**
 * 应用单例
 */
class App final : public Domain, public WithOptions {
 private:
  App();

 public:
  /**
   * 获取应用单例
   */
  static App& Inst();

  /**
   * 执行应用
   *
   * @param argc 参数个数
   * @param argv 参数列表
   * @param setup 启动函数，命令行参数分析完毕，执行指令之前调用
   */
  static int Execute(int argc, char** argv,
                     std::function<int()> setup = nullptr);

  /**
   * 设置选项结束标志
   *
   * @param end_of_options 选项结束标志
   */
  static void SetEndOfOptions(std::string const& end_of_options);

  /**
   * 设置应用名称
   *
   * @param name 名称
   */
  static void SetName(std::string const& name);

  /**
   * 设置应用简述
   */
  static void SetBrief(std::string const& brief);

  /**
   * 设置应用版本
   */
  static void SetVersion(std::string const& version);

  /**
   * 添加全局命令
   *
   * @param command 命令
   */
  static void AddGlobalCommand(Command* command);

  /**
   * 添加全局域
   *
   * @param domain 域
   */
  static void AddGlobalDomain(Domain* domain);

  /**
   * 添加全局选项
   *
   * @param opiton 选项
   */
  static void AddGlobalOption(Option* option);

  /**
   * 设置默认命令
   */
  static void SetDefaultCommand(Command* command);

 private:
  /**
   * 实际应用执行过程
   */
  int Execute(std::list<std::string>& args, std::function<int()> const& setup);

 public:
  std::string name_;                    // 应用名称
  std::optional<std::string> brief_;    // 应用简述
  std::optional<std::string> version_;  // 应用版本

  std::optional<std::string> end_of_options_;  // 选项结束标志
  Command* default_command_;                   // 默认命令
};

}  // namespace cli

#endif