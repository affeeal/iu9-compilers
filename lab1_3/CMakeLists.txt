cmake_minimum_required(VERSION 3.22)
project(lab1_3)

add_subdirectory(lib)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PRIVATE lexer)
