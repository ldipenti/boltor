# Welcome

The Boltor project's objective is to create a 2D game engine, primarily using the C language. It also aims to be an educational tool, an excuse to learn about low-level graphics programming, library development and ultimately the trade-offs needed to build a tool for game development.

As a foundation we'll be using [the awesome raylib project](https://raylib.com), so that we don't start from scratch.

## Getting started

If you have a Mac, using homebrew you need to install the following:

```
brew install llvm raylib cmake libtool
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.bash_profile
exit
```

The project has a number of config files for Visual Studio Code, some extension will get auto-installed on project open, so you can get code formatting and style checks. Also, CMake extension gets installed to be able to build the project from the editor.

## Building from the command line

If you prefer to use another IDE or just build the project directly, you can do so by executing the following from the project's root directory:

```
rm -f build && mkdir build && (cd build && cmake .. && make)
```

You'll get everything built under the `build/` directory.