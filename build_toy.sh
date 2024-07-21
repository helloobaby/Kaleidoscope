#!/bin/bash
clang++ -c $(/usr/lib/llvm-18/bin/llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native) toy.cpp -o toy.o
clang++ toy.o $(/usr/lib/llvm-18/bin/llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native) -lpthread -lncurses -o toy