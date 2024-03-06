#ifndef __CLI_DOMAIN_H__
#define __CLI_DOMAIN_H__

#include <functional>
#include <list>
#include <set>
#include <string>

namespace cli {

class Command;
class Domain;

namespace domain {
using setting = std::function<void(Domain*)>;
using settings = std::initializer_list<setting>;

/**
 * 将域绑定为另一个域的子域
 *
 * @param d 域
 */
setting bind(Domain* d);

/**
 * 为域设置名称
 *
 * @param n 名称
 */
setting name(std::string const& n);
}  // namespace domain

class Domain {
 public:
  /**
   * 构造域
   *
   * @param settings 域设置
   */
  Domain(domain::settings const& settings = {});
  Domain(Domain const&) = delete;
  Domain(Domain&&) = delete;
  Domain& operator=(Domain const&) = delete;
  Domain& operator=(Domain&&) = delete;
  virtual ~Domain() = default;

  /**
   * 添加命令
   *
   * @param command 命令
   */
  void AddCommand(Command* command);

  /**
   * 添加域
   *
   * @param domain 域
   */
  void AddDomain(Domain* domain);

  /**
   * 从参数列表中定位命令
   *
   * @param args 参数列表
   */
  Command* LocateEntry(std::list<std::string>& args);

 private:
  Command* LocateEntry(std::list<std::string>& args,
                       typename std::list<std::string>::iterator it);

 public:
  std::string name_;             // 域名称
  std::set<Command*> commands_;  // 命令
  std::set<Domain*> domains_;    // 域
};

}  // namespace cli

#endif