#ifndef __CLI_DEFINE_H__
#define __CLI_DEFINE_H__

namespace cli {

/**
 * 自动构造标记
 * 用于确保自动构造的类具备构造函数定义
 */
struct AutoConstruct {};

/**
 * 自动构造标记
 */
constexpr AutoConstruct kAutoConstruct{};

/**
 * 定义模板
 * 用于支持自动运行，在main函数运行前自动运行 Signature::__Static()
 */
template <typename Signature>
class Definition {
 public:
  Definition() {
    /**
     * 确保内部静态类不会被优化掉
     */
    if (__exec::dummy_) __exec::dummy_ = false;
  }

 private:
  struct __exec {
    static bool __Static() {
      (void)Signature::__Static();
      return true;
    }

    static bool dummy_;
  };
};

template <typename Signature>
bool Definition<Signature>::__exec::dummy_{
    Definition<Signature>::__exec::__Static()};

}  // namespace cli

#endif