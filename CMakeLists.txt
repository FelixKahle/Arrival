# Copyright 2023 WorldCourier. All rights reserved.
#
# Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

# Top level CMake file.
# This is the start of the build tree.

cmake_minimum_required(VERSION 3.16)

# Top level project.
project(ArrivalProject VERSION 1.0 LANGUAGES CXX)

# This makes C++14 a requirement and prevents a "decay"
# to C++98 when the compiler does not support C++14.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
