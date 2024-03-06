#include "cli/with_options.h"

#include <map>
#include <stdexcept>

#include "cli/app.h"
#include "cli/option.h"

namespace cli {

void WithOptions::AddOption(Option* o) { options_.emplace(o); }

void WithOptions::ParseOptions(std::list<std::string>& args) {
  /**
   * 构建关键字选项映射
   */
  std::map<std::string, Option*> keyword_options;
  for (auto o : options_) {
    for (auto k : o->keywords_) {
      keyword_options[k] = o;
    }
  }

  /**
   * 查找关键字选项
   */
  auto it = args.begin();
  auto eoo = App::Inst().end_of_options_;
  while (it != args.end() && *it != eoo) {
    auto oit = keyword_options.find(*it);
    if (oit == keyword_options.end()) {
      ++it;
      continue;
    }

    auto& o = *oit->second;

    /**
     * 重复性检查
     */
    if (!o.values_.empty() && !o.repeatable_) {
      throw std::invalid_argument{"Option " + *it + " is not repeatable"};
    }

    /**
     * 无参数选项
     */
    if (!o.arg_) {
      oit->second->values_.push_back(*it);
      it = args.erase(it);
      continue;
    }

    /**
     * 有参数选项
     */
    auto ait = std::next(it);
    if (ait == args.end()) {
      throw std::invalid_argument{"Option " + *it + " requires an argument"};
    }
    o.arg_(*ait);

    o.values_.push_back(*ait);
    args.erase(it);
    it = args.erase(ait);
  }

  /**
   * 必选选项检查
   */
  for (auto o : options_) {
    if (o->required_ && o->values_.empty()) {
      throw std::invalid_argument{"Option " + *o->keywords_.begin() +
                                  " is required"};
    }
  }
}
}  // namespace cli