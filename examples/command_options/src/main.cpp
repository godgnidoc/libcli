#include <iostream>

#include "cli/app.h"
#include "cli/cli.h"
#include "cli/command.h"
#include "cli/help.h"
#include "cli/version.h"

using namespace cli;

class HelloWorld : public CommandDefinition<HelloWorld> {
 public:
  HelloWorld()
      : CommandDefinition{cmd::global(), cmd::defaults(),
                          cmd::keyword("--hello-world"),
                          cmd::brief("Prints 'Hello, World!'")} {}

  int Execute(std::list<std::string> const& args) override {
    (void)args;
    std::cout << "Hello, World!" << std::endl;

    std::cout << std::boolalpha;

    std::cout << "verbose: " << verbose_ << std::endl;
    std::cout << "quiet: " << quiet_ << std::endl;
    std::cout << "dry_run: " << dry_run_ << std::endl;

    std::cout << "input: ";
    for (auto const& i : input_) std::cout << i << " ";
    std::cout << std::endl;
    return 0;
  }

 private:
  Option verbose_{opt::bind(this), opt::brief("Verbose output"),
                  opt::keyword("-v"), opt::keyword("--verbose")};
  Option quiet_{opt::bind(this), opt::brief("Quiet output"), opt::keyword("-q"),
                opt::keyword("--quiet")};
  Option dry_run_{opt::bind(this), opt::brief("Dry run"), opt::keyword("-n"),
                  opt::keyword("--dry-run")};
  Option input_{opt::bind(this),    opt::brief("Input file"),
                opt::keyword("-i"), opt::keyword("--input"),
                opt::repeatable(),  opt::arg()};
};

int main(int argc, char** argv) {
  App::SetVersion(PROJECT_VERSION);

  cli::DefaultVersion version{cmd::global(), cmd::keyword("--version")};
  cli::DefaultHelp help{cmd::global(), cmd::keyword("--help")};

  return App::Execute(argc, argv);
}