cmake_minimum_required(VERSION 3.25)
project(vigenere_tests)

set(CMAKE_CXX_STANDART 23)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} vigenere.cxx)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}  
    GTest::gtest_main
    GTest::gmock_main)
        
add_test(${PROJECT_NAME} ${PROJECT_NAME})