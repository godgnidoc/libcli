#include <iostream>

#include "cli/app.h"
#include "cli/cli.h"
#include "cli/help.h"
#include "cli/version.h"

using namespace cli;

struct Options : public GlobalOptions<Options> {
  Options() : GlobalOptions{kAutoConstruct} {}

  Option verbose{opt::bind(this), opt::brief("Verbose output"),
                 opt::keyword("-v"), opt::keyword("--verbose")};
  Option quiet{opt::bind(this), opt::brief("Quiet output"), opt::keyword("-q"),
               opt::keyword("--quiet")};
  Option dry_run{opt::bind(this), opt::brief("Dry run"), opt::keyword("-n"),
                 opt::keyword("--dry-run")};
  Option input{opt::bind(this),    opt::brief("Input file"),
               opt::keyword("-i"), opt::keyword("--input"),
               opt::repeatable(),  opt::arg()};
};

int main(int argc, char** argv) {
  App::SetVersion(PROJECT_VERSION);

  cli::DefaultVersion version{cmd::global(), cmd::defaults()};
  cli::DefaultHelp help{cmd::global(), cmd::keyword("help"), cmd::keyword("-h"),
                        cmd::keyword("--help")};

  return App::Execute(argc, argv, [] {
    auto& global_options = Options::Inst();
    std::cout << std::boolalpha;

    std::cout << "verbose: " << global_options.verbose << std::endl;
    std::cout << "quiet: " << global_options.quiet << std::endl;
    std::cout << "dry_run: " << global_options.dry_run << std::endl;

    std::cout << "input: ";
    for (auto const& i : global_options.input) std::cout << i << " ";
    std::cout << std::endl;

    return 0;
  });
}