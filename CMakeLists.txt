# Copyright (c) 2026 Dzmitry Minko
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.13)

project(MonsterCatVisualizerBase)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(MonsterCatVisualizerBase ${SOURCES})

target_include_directories(MonsterCatVisualizerBase PUBLIC include)

find_package(Threads REQUIRED)
target_link_libraries(MonsterCatVisualizerBase PUBLIC Threads::Threads)
