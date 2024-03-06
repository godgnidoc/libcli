#ifndef __CLI_APP_H__
#define __CLI_APP_H__

#include <map>
#include <optional>
#include <set>
#include <string>

#include "cli/cli.h"

namespace cli {
class App {
 private:
  App() = default;
  App(App const&) = delete;
  App(App&&) = delete;

  App& operator=(App const&) = delete;
  App& operator=(App&&) = delete;

  static App& Inst();

 public:
  ~App() = default;

  static void SetName(std::string const& name);
  static void SetUsage(std::string const& usage);
  static void SetVersion(std::string const& version);
  static void SetAuthor(std::string const& author);
  static void SetMain(std::string const& name);
  static void AddCmd(std::string const& name, Ctx cmd);
  static void AddOption(Opt opt);

  static std::map<std::string, Opt> Options();
  static std::map<std::string, Ctx> Commands();
  static std::optional<std::string> Name();
  static std::optional<std::string> Usage();
  static std::optional<std::string> Version();
  static std::optional<std::string> Author();
  static std::optional<std::string> Main();

  static std::optional<std::string> DocOf(Ctx cmd);
  static std::optional<std::string> BriefOf(Ctx cmd);
  static std::map<std::string, Opt> OptionsOf(Ctx cmd);
  static std::vector<Arg> ArgsOf(Ctx cmd);

  static std::optional<std::string> DocOf(Opt opt);
  static std::vector<std::string> FlagsOf(Opt opt);
  static std::vector<Arg> ArgsOf(Opt opt);
  static bool IsRequired(Opt opt);
  static bool IsRepeatable(Opt opt);

  static std::string NameOf(Arg arg);
  static bool IsOptional(Arg arg);

  static int Run(std::vector<std::string> args);

  /**
   * 下述接口供扩展 cli 框架时使用，常规应用开发无需关注
   */
  static std::set<std::string> ConsumeKeywords(std::vector<std::string>& args);
  static bool ConsumeOptions(std::map<std::string, Opt> const& options,
                             std::vector<std::string>& args);
  static bool ConsumeArguments(std::vector<Arg> crgs,
                               std::vector<std::string>& args, bool earse);

  static void SplitShorts(std::vector<std::string>& args);

  static void ClearArguments(std::vector<std::string>& args);

 private:
  std::map<std::string, Opt> options_;
  std::map<std::string, Ctx> commands_;
  std::optional<std::string> main_;
  std::optional<std::string> name_;
  std::optional<std::string> usage_;
  std::optional<std::string> version_;
  std::optional<std::string> author_;
};

int Run(int argc, char** argv);

}  // namespace cli

#endif