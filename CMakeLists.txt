cmake_minimum_required(VERSION 3.31)

# Project name and language
project(Cpu LANGUAGES CXX)

# Require modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SOURCES explicitly listed (good for reproducible builds)
set(SOURCES
  src/main.cpp
  src/devices/ppu/ppu.cpp
  src/devices/ram/ram.cpp
  src/devices/rom/rom.cpp
  src/cpu/cpu.cpp
  src/cpu/alu.cpp
  src/bus/bus.cpp
)

add_executable(app ${SOURCES})


# Expose your include directory to the compiler
target_include_directories(app
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Optional: make your include dir visible to IDEs and allow includes like "cpu/cpu.h"
target_include_directories(app SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/include)
