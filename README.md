# LIBCLI

A declarative command-line application framework for C++. Develop you CLI application efficiently using `libcli`. Define your command and options, `libcli` will parse arguments and find the correct command to execute for you.

[中文](README.zh-CN.md)

> Requirements:
> - C++ standard >= `c++17/gnu++17`
> - OS: Linux only (for now)

## Features

- Declarative pattern of usage, reduce calling of registering API.
- Command tree support, define your command under nested domains.
- Inject parsed options into specified variables, no need to find them again.
- Default help command.
- Default version command.

> Comming soon:
> - Default complete command.
