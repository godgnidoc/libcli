#ifndef __CLI_COMMAND_H__
#define __CLI_COMMAND_H__

#include <functional>
#include <initializer_list>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "cli/with_options.h"

namespace cli {

class Command;
class Domain;
class Option;

namespace cmd {

using setting = std::function<void(Command*)>;
using settings = std::initializer_list<setting>;

/**
 * 将指令绑定到全局域
 */
setting global();

/**
 * 将指令与域绑定
 *
 * @param d 域
 */
setting bind(Domain* d);

/**
 * 为指令添加简述
 *
 * @param b 简述
 */
setting brief(std::string const& b);

/**
 * 为指令添加关键字
 * 非默认指令应当至少有一个关键字
 *
 * @param k 关键字
 */
setting keyword(std::string const& k);

/**
 * 为指令添加参数说明
 * 拥有参数说明的指令才会接受剩余参数
 *
 * @param a 参数说明
 */
setting args(std::string const& a = "...");

/**
 * 将指令标记为默认指令
 */
setting defaults();

}  // namespace cmd

/**
 * 指令抽象
 */
class Command : public WithOptions {
 public:
  /**
   * 构造时传入设置列表
   *
   * @param settings 设置列表
   */
  Command(cmd::settings const& settings);
  Command(Command const&) = delete;
  Command(Command&&) = delete;
  Command& operator=(Command const&) = delete;
  Command& operator=(Command&&) = delete;
  virtual ~Command() = default;

  /**
   * 执行指令
   *
   * @param args 剩余参数
   */
  virtual int Execute(std::list<std::string> const& args) = 0;

 public:
  std::optional<std::string> args_;  // 参数说明，有说明的指令才会接受剩余参数
  std::optional<std::string> brief_;  // 简述
  std::set<std::string> keywords_;    // 能够触发当前指令的关键字
};

}  // namespace cli

#endif