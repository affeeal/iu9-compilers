cmake_minimum_required(VERSION 3.22)
project(lab1-2 LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS regex)

add_subdirectory(lib)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(${PROJECT_NAME} PRIVATE lexer)
