cmake_minimum_required(VERSION 3.16)
project(nbtpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

enable_testing()

add_subdirectory(test)
add_library(nbtpp STATIC include/nbtpp.h src/nbtpp.cpp include/Tags.h src/Tags.cpp include/RootCompound.h src/RootCompound.cpp include/Hex.h src/Hex.cpp include/Edition.h include/TagID.h include/payload.h)
