cmake_minimum_required(VERSION 3.10)

project(HttpsServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(include)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(HttpsServer ${SOURCES})

