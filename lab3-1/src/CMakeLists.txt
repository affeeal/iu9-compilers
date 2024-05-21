cmake_minimum_required(VERSION 3.22.1)
project(lab3-1)

find_package(FLEX REQUIRED)

flex_target(scanner lexer/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)

add_executable(${PROJECT_NAME} main.cc ${FLEX_scanner_OUTPUTS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_subdirectory(lexer)
add_subdirectory(parser)
