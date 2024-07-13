cmake_minimum_required(VERSION 3.25)
project(Crypto)

set(CMAKE_CXX_STANDART 23)

find_package(cryptopp REQUIRED)
find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Stb REQUIRED)

add_executable(${PROJECT_NAME} src/main.cxx)
target_link_libraries(${PROJECT_NAME} PUBLIC
    cryptopp::cryptopp
    imgui::imgui 
    ${GLFW_LIBRARY}
    ${OPENGL_LIBRARY})

enable_testing()
add_subdirectory(test)
