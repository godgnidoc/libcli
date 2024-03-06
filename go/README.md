<h1>Reflecting CLI</h1>

反射CLI是一款声明式Go语言命令行框架，追求接口的简单易用。支持如下功能：

- 支持解析标准长选项和标准短选项
- 以函数为中心定义命令，选项和参数
- 以上下文为中心定义命令，选项和参数
- 内置命令自动生成帮助页
- > TODO: 内置命令自动生成补全脚本

# 文档目录

- [文档目录](#文档目录)
- [命令声明](#命令声明)
  - [以函数为中心](#以函数为中心)
  - [以上下文为中心](#以上下文为中心)
- [具名参数](#具名参数)
  - [以上下文为中心](#以上下文为中心-1)
  - [剩余参数](#剩余参数)
  - [以函数为中心](#以函数为中心-1)
- [选项声明](#选项声明)
  - [以上下文为中心](#以上下文为中心-2)
  - [以函数为中心](#以函数为中心-2)
- [其它功能](#其它功能)
  - [应用校验](#应用校验)
  - [应用执行](#应用执行)
  - [帮助命令](#帮助命令)
  - [命令文档](#命令文档)
  - [应用信息](#应用信息)
  - [应用版本](#应用版本)

# 命令声明

`cli` 框架支持以函数为中心和以上下文为中心两种命令声明方法。

- 以函数为中心的声明方式简洁快速，适合随手写的简单应用开发。
- 以上下文为中心的命令声明能够支撑更加复杂的命令设计，适用于正式工具开发。

## 以函数为中心

使用 `cli.CmdFn` 接口可以快速声明一个命令：

```go
// app.go
package main

import "github.com/godgnidoc/cli"

func MyCmd(args []string) int {
    // Do something
}

func main() {
    // ./app cmd fn
    cli.CmdFn(MyCmd)

    cli.Run()
}
```

上述代码定义了一个指令 `my cmd` 使用方法如下：

```bash
./app my cmd
```

`cli.CmdFn` 接口会将驼峰命名风格的函数名拆解用作命令关键字，`cli.Run` 接口解析命令行参数，找到并执行指定的命令。

命令回调的参数是去掉选项和命令关键字之后的剩余参数。命令回调的返回值被用作进程退出码。

我们也可以多次调用 `cli.CmdFn` 接口定义更多指令，指令的关键字前缀可以重复，自然形成命令树。


```go
// app.go
package main

import "github.com/godgnidoc/cli"

func AddUser(args []string) int {
    // Do something
}

func AddGroup(args []string) int {
    // Do something
}

func main() {
    // ./app add user
    cli.CmdFn(AddUser)

     // ./app add group
    cli.CmdFn(AddGroup)

    cli.Run()
}
```

注意，`CmdFn` 接口需要函数的名字作为命令关键字，所以匿名函数不能被用作命令回调。

使用 `cli.MainFn` 可以定义一个主命令，当命令行参数不指定任何命令时，回滚到执行主命令。

```go
// app.go
package main

import "github.com/godgnidoc/cli"

func AddUser(args []string) int {
    // Do something
}

func AddGroup(args []string) int {
    // Do something
}

func ShowStatus(args []stirng) int {
    // Do something
}

func main() {
    // ./app add user
    cli.CmdFn(AddUser) 
    
    // ./app add group
    cli.CmdFn(AddGroup) 
    
    // ./app
    // ./app show status
    cli.MainFn(ShowStatus)

    cli.Run()
}
```

使用 `MainFn` 定义的命令依然可以使用原本的关键字执行。

## 以上下文为中心

以上下文为中心定义命令的风格适用于较为复杂的场景，比如命令较多，选项或参数也很多的情况。

本节的话题尚未涉及参数和选项的声明，此处仅展示以上下文为中心声明命令的基本方法。

```go
// app.go
package main

import "github.com/godgnidoc/cli"

type AddUser {
  
}

func (cmd *AddUser) Run(args []string) int {
    // ...
}

type AddGroup {
  
}

func (cmd *AddGroup) Run(args []string) int {
    // ...
}

func main() {
    // ./app add user
    cli.Cmd(&AddUser{
        // ...
    })

    // ./app add group
    cli.Cmd(&AddGroup{
        // ...
    })

    cli.Run()
}

```
`Cmd`接口接受一个上下文指针来创建指令，使用上下文类型名生成命令关键字。

命令上下文需要实现一个 `Run` 方法，参数为剔除选项和具名参数以及命令关键字后的剩余命令行参数。返回值被用作进程退出码。

# 具名参数

命令可以接受若干按顺序指定的参数，命令回调的形参过于简单，当命令需要处理多个参数时，反复使用下标访问参数列表会严重降低代码的可读性。

## 以上下文为中心

可以为上下文命令定义具名参数，框架会将成功分析的具名参数存入指定变量，多出来的剩余参数才会被传入命令入口函数。

```go
// app.go
package main

import "github.com/godgnidoc/cli"

type AddUser {
  UserName *cli.Arg
  GroupName *cli.Arg
}

func (cmd *AddUser) Run(args []string) int {
    // ...
    user_name := cmd.UserName.Value()
    if cmd.GroupName.HasValue() {
        group_name := cmd.GroupName.Value()
        // ...
    }
}

type AddGroup {
  GroupName *cli.Arg
}

func (cmd *AddGroup) Run(args []string) int {
    // ...
    group_name := cmd.GroupName.Value()
}

func main() {
    // ./app add user myuser
    // ./app add user myuser mygroup
    cli.Cmd(&AddUser{
        UserName: cli.Argument("user-name"),
        GroupName: cli.Optional("group-name")
    })

    // ./app add group mygroup
    cli.Cmd(&AddGroup{
        GroupName: cli.Argument("group-name"),
    })

    cli.Run()
}
```

可以用 `*cli.Arg` 类型在命令上下文中定义任意个参数，它们必须都是公开成员。框架会根据这些参数字段的定义顺序解析命令行参数。

命令参数分为可选参数和必须参数，如果命令行没有给足必须参数，则框架会报告错误并提前退出。

> 可选参数出现后，不能再出现必须参数。出于效率考虑，cli 不会每次主动检查此错误。用户可以在开发阶段使用 `cli.ValidateApp()` 来校验应用定义的正确性。

参数定义除了用于指导命令行解析以外，还参与构建帮助页面。必须参数被显示为 `<arg>` 形式，可选参数被显示为 `<arg>` 形式。

## 剩余参数

框架总是会把剔除命令关键字、选项和具名参数之后剩余的命令行参数传递给命令回调函数。

如果我们确实总是需要用到剩余参数，且希望在帮助页告诉用户可以放心大胆的传入若干参数，可以使用 `More` 接口定义一个剩余参数。

剩余参数之后不能出现任何参数定义。

```go
// app.go
package main

import "github.com/godgnidoc/cli"

type Compile struct {
    Source *cli.Arg
    More *cli.Arg
}


func (cmd *Compile) Run(args []string) int {
    // ...
    sources := append([]string{cmd.Source.Value()}, args...)
}

func main() {
    // ./app compile firstSource
    // ./app compile firstSource SecondSource ThirdSource
    cli.Cmd(&Compile{
        Source: cli.Argument("source"),
        More: cli.More()
    })

    cli.Run()
}
```

如上例子演示了如何使用剩余参数，同时也暴露了一个问题。如果剩余参数和最后一个必须参数实际上是同一组数据，我们要将它们合并再处理就会麻烦。

因此，我们引入和 `Some` 接口，可以为命令定义要求至少出现一次的剩余参数。

```go
// app.go
package main

import "github.com/godgnidoc/cli"

type Compile struct {
    Source *cli.Arg
}


func (cmd *Compile) Run(args []string) int {
    // ...
    for source := range cmd.Source.All() {
        // ...
    }
}

func main() {
    // ./app compile firstSource
    // ./app compile firstSource SecondSource ThirdSource
    cli.Cmd(&Compile{
        Source: cli.Some("source"),
    })

    cli.Run()
}
```

使用 `Some` 接口定义的剩余参数相当于合并了一个必须参数和一系列剩余参数。使用 `All` 可以获取其中存储的全部值。

## 以函数为中心

以函数为中心的命令也可以使用接口定义具名参数，但我们拿不到这些定义的指针，所以也无从获取存储其中的值。

因此，框架不会将以函数为中心的命令的具名参数从命令行剔除，但是依然会为它们生成帮助文档，以及检查命令行参数是否够用。

```go
// app.go
package main

import "github.com/godgnidoc/cli"

func Compile(args []string) int {
    // ...
    for source := range args {
        // ...
    }
}

func Compare(args []string) int {
    first := args[0]
    second := args[1]
    // ...
}

func Setup(args []string) int {
    if len(args) > 1 {
        taget := args[0]
        // ...
    }
}

func main() {
    // ./app compile firstSource
    // ./app compile firstSource SecondSource ThirdSource
    cli.CmdFn(Compile).
        Some("source")

    // ./app compare firstValue SecondValue
    cli.CmdFn(Compare).
        Arg("first").
        Arg("second")

    // ./app setup
    // ./app setup mytarget
    cli.CmdFn(Setup).
        Opt("target")

    cli.Run()
}
```

# 选项声明

参数的指定是有顺序要求的，而选项则不要求传入顺序。`cli`支持为指令定义选项并绑定到一个变量上。选项有如下属性可以设置：

- 选项至少要有一个关键字
- 是否必须，默认可选
- 是否可重复，默认不可以
- 选项也可以携带任意个具名参数，但是仅支持必须参数
- 选项可以有描述文本用于生成帮助页

框架会从命令行分析选项是否被指定，每次指定携带的参数。

## 以上下文为中心

以上下文为中心的命令，可以在上下文结构中使用 `*cli.Opt` 类型声明任意个公开的选项变量并指定具体属性。

框架会在命令被关键字选中后，反射出命令上下文结构中的选项定义。并尝试从命令行参数解析选项和它们携带的参数存入选项变量。

被成功分析的选项会从命令行参数中被移除，若分析到参数缺失，或选项缺失或不合理的选项重复，则框架会报告错误并提前退出。

```go
// app.go
package main

import "github.com/godgnidoc/cli"

type Compile struct {
    DebugInfo *cli.Opt
    OutputDir *cli.Opt
    IncludeDir *cli.Opt
    Source *cli.Arg
}

func (cmd *Compile) Run(args []string) int {
    // ...
    debug := cmd.DebugInfo.IsOn()
    includes := cmd.IncludeDir.AllArgs()
    output := cmd.OutputDir.AllArgs()[0]
}

func main() {
    cli.Cmd(&Compile{
        DebugInfo : cli.Option("-d", "--debug").
            Doc("Generate debug info"),
        OutputDir: cli.Option("-o", "--output").
            Arg("path").
            Required().
            Doc("Specify the output path of binary"),
        IncludeDir: cli.Option("-I", "--include").
            Arg("path").
            Repeatable().
            Doc("Add include path"),
        Source: cli.Some("source")
    })

    cli.Run()
}
```

框架为选项提供了一系列方法用于访问存储其中的数据：

- `IsOn` 用于判断此选项是否至少被指定过一次
- `AllArgs` 把每次指定时携带的参数收集到一个数组中获取
- `Times` 获取当前选项的指定次数
- `ArgGroup` 获取第i次指定当前选项时携带的参数列表

## 以函数为中心

以函数为中心的命令也可以定义选项。我们需要先定义好选项指针，把 `Option` 接口返回的指针保存起来。之后再使用 `Use` 接口告诉框架当前命令使用哪些选项。

```go
// app.go
package main

import "github.com/godgnidoc/cli"

var DebugInfo = cli.Option("-d", "--debug").
    Doc("Generate debug info")
var OutputDir = cli.Option("-o", "--output").
    Arg("path").
    Required().
    Doc("Specify the output path of binary")
var IncludeDir = cli.Option("-I", "--include").
    Arg("path").
    Repeatable().
    Doc("Add include path")

func Compile(args []string) int {
    // ...
    debug := DebugInfo.IsOn()
    includes := IncludeDir.AllArgs()
    output := OutputDir.AllArgs()[0]
}

func main() {
    cli.Cmd(Compile).
        Use(DebugInfo).
        Use(OutputDir).
        Use(IncludeDir).
        Some("source")

    cli.Run()
}
```

聪明的你可能已经注意到了，预先定义的选项变量可以被多个命令通过 `Use` 接口绑定起来。这样可以复用选项定义。

这是一个非常聪明的想法，但是这对选项的设计和版本管理有更高的要求。请慎重使用。

# 其它功能

## 应用校验

应用设计需要遵循一些规则，比如长短选项的语法规则，可选参数之后不能定义必须参数等。

有些规则不能通过编译器静态检查，为了性能考虑，框架也不能每次自动执行运行时检查。

所以，用户可以在开发环境下，调用 `cli.ValidateApp` 接口检查应用的设计是否合理。待发布应用时，去掉此调用即可。

## 应用执行

调用 `cli.Run` 接口会开始分析命令行参数，执行指定的命令。待命令执行完毕，框架会直接使用命令回调的返回值做退出码结束进程。

`cli.Run` 默认使用 `os.Args[1:]` 作为命令行参数。我们可以手动传入自定义的命令行参数：

```go
cli.Run(&[]string{"this", "is", "my", "args"})
```

## 帮助命令

`cli` 框架内置了一个基于上下文的 `Help` 命令用于从当前的应用设计生成并显示帮助信息。

`Help` 命令可以展示应用整体的使用说明。也可以展示某个命令的详细说明。

我们需要手动启用帮助命令：

```go
func main() {
    cli.Compile(Compile)
    cli.Cmd(cli.Help)
    cli.Run()
}
```
## 命令文档

前文已经展示了如何在定义选项时顺便指定帮助文本。我们还可以为命令指定帮助文本。

与选项不同，命令支持设置两份帮助文档，即 `Brief` 和 `Doc` 。当帮助页要显示整体说明时，会在命令列表中展示每个命令的 `Brief`。当帮助页要展示某个命令的详细说明时，会打印 `Doc` 文本。

我们可以在命令上下文中定义 `Brief` 和 `Doc` 变量，框架找到这两个变量后就会在恰当的时机打印它们。

```go
type Compile struct {
    DebugInfo *cli.Opt
    OutputDir *cli.Opt
    IncludeDir *cli.Opt
    Source *cli.Arg
    Brief string
    Doc string
}

func (cmd *Compile) Run(args []string) int {
    // ...
}

func main() {
    cli.Cmd(&Compile{
        // ...
        Brief: "Short brief of command",
        Doc: "This is the long doc\n" +
            "Which could have multiple lines"
    })

    cli.Run()
}
```
对于基于函数的命令，`cli`框架页对应提供了 `Brief` 和 `Doc` 接口用于指定说明文本：

```go
func Compile(args []string) int {
    // ...
}

func main() {
    cli.Cmd(Compile).
        Brief("Short brief of command").
        Doc("This is the long doc\n" +
            "Which could have multiple lines")

    cli.Run()
}
```

## 应用信息

帮助命令会在一些页面展示应用信息，我们可以设置如下变量来定制这些页面上的信息：

```go
func main() {
    cli.AppName = "myapp"
    cli.AppUsage = "Just for fun"
}
```

## 应用版本

`cli` 还提供了另外一个命令 `Version` 用于打印应用版本信息，同样需要手动启用它：

```go
func main() {
    cli.Cmd(cli.Version)

    cli.Run()
}
```

`Version` 命令会打印如下可以定制的信息：

```go
func main() {
    cli.AppName = "myapp"
    cli.AppAuthor = "myname <myname@mymail.com>"
    cli.Version = "1.0.0"
}
```