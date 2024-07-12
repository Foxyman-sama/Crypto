cmake_minimum_required(VERSION 3.25)
project(Crypto)

set(CMAKE_CXX_STANDART 23)

find_package(cryptopp REQUIRED)

add_executable(${PROJECT_NAME} src/main.cxx)
target_link_libraries(${PROJECT_NAME} PUBLIC
    cryptopp::cryptopp)

enable_testing()
add_subdirectory(test)
