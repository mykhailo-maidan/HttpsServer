cmake_minimum_required(VERSION 3.5)

project(HttpsServer LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HttpsServer main.cpp ServerSocket.cpp Socket.cpp)

add_executable(HttpsClient clientExample/main.cpp)

target_link_libraries(HttpsServer ${CONAN_LIBS})

target_link_libraries(HttpsClient ${CONAN_LIBS})
