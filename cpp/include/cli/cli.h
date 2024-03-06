#ifndef __CLI_H__
#define __CLI_H__

#include <memory>
#include <string>
#include <vector>

/**
 * forward declarations
 */
namespace cli {

class CmdDef;
class OptDef;
class ArgDef;
class FnCommand;

using Ctx = std::shared_ptr<CmdDef>;
using Opt = std::shared_ptr<OptDef>;
using Arg = std::shared_ptr<ArgDef>;

using Fn = int(std::vector<std::string>);

}  // namespace cli

#include "cli/app.h"
#include "cli/arg.h"
#include "cli/cmd.h"
#include "cli/fn.h"
#include "cli/opt.h"

/**
 * implementation comes after the interface
 */

#include "cli/details/app.inl"
#include "cli/details/arg.inl"
#include "cli/details/cmd.inl"
#include "cli/details/fn.inl"
#include "cli/details/opt.inl"

#endif