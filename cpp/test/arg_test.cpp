#include "cli/cli.h"
#include "gtest/gtest.h"

namespace arg_test {

class MyCmd : public cli::CmdDef {
 public:
  MyCmd(){}

  int Run(std::vector<std::string>) override {
    return 0;
  }

 public:
  cli::Arg first = Argument("first");
  cli::Arg second = Argument("second");
  cli::Arg some = Some("some");
};

TEST(ArgTest, Contextual) {
  cli::Cmd<MyCmd>();
  EXPECT_EQ(-1, cli::App::Run({"my", "cmd"}));
}

TEST(ArgTest, ContextualWithArg) {
  cli::Cmd<MyCmd>();
  EXPECT_EQ(0, cli::App::Run({"my", "cmd", "first", "second", "one", "two"}));

  auto& cmd = *std::dynamic_pointer_cast<MyCmd>(cli::App::Commands()["MyCmd"]);
  EXPECT_EQ("first", cmd.first->Value());
  EXPECT_EQ("second", cmd.second->Value());
  EXPECT_EQ(2, cmd.some->Values().size());
  EXPECT_EQ("one", cmd.some->Values()[0]);
  EXPECT_EQ("two", cmd.some->Values()[1]);
}

}  // namespace arg_test