#include "cli/cli.h"
#include "gtest/gtest.h"

namespace cmd_test {

class MyCmd : public cli::CmdDef {
 public:
  MyCmd(std::string& data) : data_(data) {}

  int Run(std::vector<std::string> args) override {
    data_ = args[0];
    return 0;
  }

 private:
  std::string& data_;
};

TEST(CmdTest, Contextual) {
  std::string data;
  cli::Cmd<MyCmd>(data);

  int argc = 4;
  char* argv[] = {
      (char*)"./test",
      (char*)"my",
      (char*)"cmd",
      (char*)"data",
  };

  EXPECT_EQ(0, cli::Run(argc, argv));
  EXPECT_EQ("data", data);
}

int MyFnCmd(std::vector<std::string> const& args) {
  return std::stoll(args[0]);
}

TEST(CmdTest, Functional) {
  cli::CmdFn("MyFnCmd", MyFnCmd);

  int argc = 5;
  char* argv[] = {
      (char*)"./test", (char*)"my", (char*)"fn", (char*)"cmd", (char*)"42",
  };

  EXPECT_EQ(42, cli::Run(argc, argv));
}

}  // namespace cmd_test
