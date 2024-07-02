cmake_minimum_required(VERSION 3.25)
project(Crypto)

set(CMAKE_CXX_STANDART 23)

add_executable(${PROJECT_NAME} src/main.cxx)

enable_testing()
add_subdirectory(test)
