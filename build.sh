#!/bin/bash

rm -rf CMakeCache.txt CMakeFiles Makefile cmake_install.cmake process_image
cmake -D CMAKE_BUILD_TYPE=RELEASE .
make -j2

