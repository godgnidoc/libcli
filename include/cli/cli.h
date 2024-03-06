#include "cli/app.h"
#include "cli/command.h"
#include "cli/define.h"
#include "cli/domain.h"

namespace cli {

/**
 * 将选项集内的选项绑定到全局选项集
 *
 * @tparam Options 全局选项集类型
 */
template <typename Options>
class GlobalOptions : public WithOptions,
                      public Definition<GlobalOptions<Options>> {
 public:
  /**
   * 自动构造要求有构造函数
   */
  GlobalOptions(AutoConstruct const&) {}

  /**
   * 获取全局选项集单例
   */
  static Options& Inst() {
    static Options options;
    return options;
  }

 public:
  /**
   * 自动注册全局选项
   */
  static void __Static() {
    auto& inst = GlobalOptions::Inst();
    for (auto option : inst.options_) App::AddGlobalOption(option);
  }
};

/**
 * 自动注册为全局域的域定义
 *
 * @tparam D 域类型
 */
template <typename D>
class DomainDefinition : public Domain, Definition<DomainDefinition<D>> {
 public:
  /**
   * 构造域定义
   */
  DomainDefinition(AutoConstruct const&) {}

 public:
  static void __Static() {
    static D domain;
    App::AddGlobalDomain(domain);
  }
};

/**
 * 自动注册的全局指令定义
 *
 * @tparam Cmd 指令类型
 */
template <typename Cmd>
class CommandDefinition : public Command, Definition<CommandDefinition<Cmd>> {
 public:
  /**
   * 自动构造要求有构造函数
   * 此参数用于确保显式定义构造函数
   */
  CommandDefinition(cmd::settings const& settings) : Command(settings) {}

 public:
  static void __Static() {
    static Cmd command;
    App::AddGlobalCommand(&command);
  }
};

}  // namespace cli