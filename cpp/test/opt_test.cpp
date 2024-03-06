#include "cli/cli.h"
#include "gtest/gtest.h"

namespace opt_test {

struct MyCmd : public cli::CmdDef {
 public:
  int Run(std::vector<std::string> args) override {
    args_ = args;
    return 0;
  }

 public:
  std::vector<std::string> args_;
  cli::Opt optional_ = Option("-o", "--optional");
  cli::Opt required_ = Option("-r", "--required")->Required();
  cli::Opt repeatable_ = Option("-p", "--repeatable")->Repeatable()->Arg("arg");
};

TEST(OptTest, None) {
  cli::Cmd<MyCmd>();
  auto& cmd = *std::dynamic_pointer_cast<MyCmd>(cli::App::Commands()["MyCmd"]);
  EXPECT_EQ(-1, cli::App::Run({"my", "cmd"}));
  EXPECT_EQ(0, cmd.args_.size());
  EXPECT_FALSE(cmd.optional_->IsOn());
  EXPECT_FALSE(cmd.required_->IsOn());
  EXPECT_EQ(0, cmd.repeatable_->Times());
}

TEST(OptTest, Required) {
  cli::Cmd<MyCmd>();
  auto& cmd = *std::dynamic_pointer_cast<MyCmd>(cli::App::Commands()["MyCmd"]);
  EXPECT_EQ(0, cli::App::Run({"my", "cmd", "-r"}));
  EXPECT_EQ(0, cmd.args_.size());
  EXPECT_FALSE(cmd.optional_->IsOn());
  EXPECT_TRUE(cmd.required_->IsOn());
  EXPECT_EQ(0, cmd.repeatable_->Times());
}

TEST(OptTest, Optional) {
  cli::Cmd<MyCmd>();
  auto& cmd = *std::dynamic_pointer_cast<MyCmd>(cli::App::Commands()["MyCmd"]);
  EXPECT_EQ(0, cli::App::Run({"my", "cmd", "-o", "--required"}));
  EXPECT_EQ(0, cmd.args_.size());
  EXPECT_TRUE(cmd.optional_->IsOn());
  EXPECT_TRUE(cmd.required_->IsOn());
  EXPECT_EQ(0, cmd.repeatable_->Times());
}

TEST(OptTest, Repeatable) {
  cli::Cmd<MyCmd>();
  auto& cmd = *std::dynamic_pointer_cast<MyCmd>(cli::App::Commands()["MyCmd"]);
  EXPECT_EQ(0, cli::App::Run({"my", "cmd", "--required", "-p", "one",
                              "-p", "two"}));
  EXPECT_EQ(0, cmd.args_.size());
  EXPECT_FALSE(cmd.optional_->IsOn());
  EXPECT_TRUE(cmd.required_->IsOn());
  EXPECT_EQ(2, cmd.repeatable_->Times());
  EXPECT_EQ(2, cmd.repeatable_->AllArgs().size());
  EXPECT_EQ("one", cmd.repeatable_->ArgsAt(0)[0]);
  EXPECT_EQ("two", cmd.repeatable_->ArgsAt(1)[0]);
}

}  // namespace opt_test
