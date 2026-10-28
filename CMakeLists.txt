cmake_minimum_required(VERSION 3.13)

project(MonsterCatVisualizerBase)
set(CMAKE_CXX_STANDARD 17)

add_library(MonsterCatVisualizerBase)

target_sources(MonsterCatVisualizerBase
  PRIVATE
    src/animator.cpp
    src/particle.cpp
    src/cloud.cpp
    src/line.cpp
    src/cloudsContainer.cpp
    src/linesContainer.cpp
    src/particlesContainer.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/helpers.h
      include/animator.h
      include/particle.h
      include/cloud.h
      include/line.h
      include/particlesContainer.h
      include/cloudsContainer.h
      include/linesContainer.h
)