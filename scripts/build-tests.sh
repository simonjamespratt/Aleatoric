#!/usr/bin/env bash

# This script is based on the following: https://mottosso.gitbooks.io/clang/building_a_static_library.html
# Add a shell script as a task for VS Code as follows: https://code.visualstudio.com/docs/editor/tasks

# make the folders if they don't already exist
mkdir -p build/debug
# pushd and popd are bash commands
# using pushd <dir> will take you to the directory specified (as if you had done `cd <dir>`) and add a reference to the place of departure to the "stack"
# using popd after that will return you to the place you came from (referenced in the stack), and pop that ref off the stack
pushd build/debug

# Clang commands:
# https://clang.llvm.org/docs/ClangCommandLineReference.html
# -c means --compile
# -Wall means "warnings all" - enable all compiler warnings
# -o<file> means --ouput<file> write the output to the specified file
# -I<dir> means --include-directory <dir> the path to the headers to include
# the final line is the source code to compile

# The && at the end of the final line is a shell script command. It means "do the next command is this command has been successful".
# The backslashes are also shell script commands.
# They allow long commands to be split onto different lines without actually creating a newline in the end version of the command.

clang++ \
    -std=c++14 \
    -stdlib=libc++ \
    -Wall \
    -o main \
    -I ../../include/actlib \
    -I ../../tests/FakeIt/single_header/catch \
    -I ../../tests \
    ../../tests/main.cpp \
    libAleatoricCompositionTools.a \
    --debug

popd
