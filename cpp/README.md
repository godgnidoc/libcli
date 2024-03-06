# LIBCLI

`libcli` 被设计为一款声明式C++命令行工具开发框架。使用 `libcli` 可以快速开发命令行应用程序。定义你的指令和选项，`libcli` 会帮你分析命令行参数，找到并执行正确的指令。

[English](README.md)

> Requirements:
> - C++ standard >= `c++17/gnu++17`
> - OS: Linux only (for now)

## 特性

- API 声明式设计，显著减少注册接口调用频率。
- 指令树支持，可以将指令定义在多层嵌套的域中。
- 将分析过的命令行参数注入指定变量，方便使用。
- 默认帮助指令

> Comming soon:
> - 默认命令补全指令
