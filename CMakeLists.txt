cmake_minimum_required(VERSION 3.31)

# Project name and language
project(Cpu LANGUAGES CXX)

# Require modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add an executable from source files
add_executable(app
  src/main.cpp
  src/devices/ppu/ppu.cpp
  src/devices/ram/ram.cpp
  src/devices/rom/rom.cpp
  src/cpu/cpu.cpp
  src/cpu/alu.cpp
  src/bus/bus.cpp
)
