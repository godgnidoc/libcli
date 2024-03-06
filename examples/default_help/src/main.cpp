#include "cli/app.h"
#include "cli/help.h"

int main(int argc, char **argv) {
  using namespace cli;

  DefaultHelp help{cmd::global(), cmd::keyword("help"), cmd::keyword("-h"),
                   cmd::keyword("--help")};

  return App::Execute(argc, argv);
}