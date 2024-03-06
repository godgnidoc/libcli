#ifndef __CLI_WITH_OPTIONS_H__
#define __CLI_WITH_OPTIONS_H__

#include <list>
#include <set>
#include <string>

namespace cli {

class Option;

/**
 * 选项集
 */
class WithOptions {
 public:
  virtual ~WithOptions() = default;

  /**
   * 添加选项
   *
   * @param o 选项
   */
  void AddOption(Option* o);

  /**
   * 分析选项
   *
   * @param args 参数列表
   */
  void ParseOptions(std::list<std::string>& args);

 public:
  std::set<Option*> options_;
};

}  // namespace cli

#endif