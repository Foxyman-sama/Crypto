cmake_minimum_required(VERSION 3.25)
project(aes_tests)

set(CMAKE_CXX_STANDART 23)

find_package(GTest REQUIRED)
find_package(cryptopp REQUIRED)

add_executable(${PROJECT_NAME} aes.cxx)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}  
    GTest::gtest_main
    GTest::gmock_main
    cryptopp::cryptopp)
        
add_test(${PROJECT_NAME} ${PROJECT_NAME})