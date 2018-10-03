cmake_minimum_required(VERSION 2.6)
project(cpuid)

add_executable(cpuid main.cpp)

install(TARGETS cpuid RUNTIME DESTINATION bin)
