#!/bin/bash

rm -rf CMakeCache.txt CMakeFiles Makefile cmake_install.cmake gamma_correction
cmake -D CMAKE_BUILD_TYPE=RELEASE .
make -j2

