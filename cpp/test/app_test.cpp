#include "cli/cli.h"
#include "cli/version.h"
#include "gtest/gtest.h"

namespace app_test {

TEST(AppTest, Empty) {
  int argc = 1;
  char* argv[] = {(char*)"test"};
  EXPECT_EQ(-1, cli::Run(argc, argv));
}

TEST(AppTest, Main) {
  cli::Main<cli::Version>();
  int argc = 1;
  char* argv[] = {(char*)"test"};
  EXPECT_EQ(0, cli::Run(argc, argv));
}

TEST(AppTest, Version) {
  cli::Main<cli::Version>();
  int argc = 2;
  char* argv[] = {(char*)"test", (char*)"version"};
  EXPECT_EQ(0, cli::Run(argc, argv));
}

}  // namespace app_test
