#ifndef __CLI_OPTION_H__
#define __CLI_OPTION_H__

#include <functional>
#include <initializer_list>
#include <list>
#include <optional>
#include <set>
#include <string>

namespace cli {
class Option;
class WithOptions;

namespace opt {

using setting = std::function<void(Option*)>;
using settings = std::initializer_list<setting>;
using arg_validator = std::function<void(std::string const&)>;

/**
 * 为选项添加简述
 *
 * @param b 简述
 */
setting brief(std::string const& b);

/**
 * 使选项成为必选项
 */
setting required();

/**
 * 使选项可重复
 */
setting repeatable();

/**
 * 为选项添加关键字
 *
 * @param k 关键字
 */
setting keyword(std::string const& k);

/**
 * 为选项添加参数候选列表
 * 令选项必须携带一个参数，且参数必须在候选列表中
 *
 * @param a 参数候选列表
 */
setting arg(std::set<std::string> const& a);

/**
 * 为选项添加参数验证器
 * 令选项必须携带一个参数，且参数必须通过验证器
 *
 * @param a 参数验证器
 */
setting arg(arg_validator a);

/**
 * 为选项添加一个参数验证器
 * 令选项必须携带一个参数
 */
setting arg();

/**
 * 将选项绑定到选项集
 *
 * @param o 选项集
 */
setting bind(WithOptions* o);

/**
 * 将选项绑定到全局选项集
 */
setting global();

}  // namespace opt

class Option final {
  friend class WithOptions;

 public:
  /**
   * 构造选项
   *
   * @param settings 选项设置
   */
  Option(opt::settings const& settings);
  Option(Option const&) = delete;
  Option(Option&&) = delete;
  Option& operator=(Option const&) = delete;
  Option& operator=(Option&&) = delete;
  ~Option() = default;

  /**
   * 选项被指定过至少一次
   */
  operator bool() const;

  /**
   * 获取选项值
   *
   * @param i 索引
   * @return 选项值
   */
  std::string const& operator[](std::size_t i) const;

  /**
   * 获取选项值数量
   *
   * @return 选项值数量
   */
  size_t size() const;

  /**
   * 获取选项值迭代器
   *
   * @return 选项值迭代器
   */
  typename std::vector<std::string>::const_iterator begin() const;

  /**
   * 获取选项值迭代器
   *
   * @return 选项值迭代器
   */
  typename std::vector<std::string>::const_iterator end() const;

  /**
   * 获取选项第一个值的引用
   */
  std::string const& operator*() const;

  /**
   * 获取选项第一个值的指针
   */
  std::string const* operator->() const;

 public:
  std::optional<std::string> brief_;  // 简述
  bool required_{false};              // 是否必选
  bool repeatable_{false};            // 是否可重复
  std::set<std::string> keywords_;    // 关键字
  opt::arg_validator arg_;            // 参数验证器

 private:
  std::vector<std::string> values_;  // 值
};

}  // namespace cli

#endif