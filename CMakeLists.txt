cmake_minimum_required(VERSION 3.20.1)

project(SubdomainJob)
add_definitions("-std=c++11")

add_executable(${PROJECT_NAME} main.cpp)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) # Include Conan-generated file
conan_basic_setup(TARGETS) # Introduce Conan-generated targets

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

