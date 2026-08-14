cmake_minimum_required(VERSION 3.22.1)
project(calculator LANGUAGES CXX)

find_package(FLEX REQUIRED)

flex_target(scanner lexer/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)

add_executable(${PROJECT_NAME} main.cc ${FLEX_scanner_OUTPUTS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(semantics)

include(CTest)
if(BUILD_TESTING)
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  add_test(
    NAME calculator-smoke
    COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../tests/check_calculator.py
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_CURRENT_SOURCE_DIR}/../examples/example.txt
  )
endif()
