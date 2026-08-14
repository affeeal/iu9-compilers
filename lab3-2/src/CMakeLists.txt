cmake_minimum_required(VERSION 3.22.1)
project(lab3-2 LANGUAGES CXX)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

flex_target(scanner
  scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc
)

bison_target(parser
  parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
)

add_flex_bison_dependency(scanner parser)

add_executable(${PROJECT_NAME} 
  ast.cc
  driver.cc
  formatter.cc
  ident_table.cc
  main.cc
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(CTest)
if(BUILD_TESTING)
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  add_test(
    NAME lab3-2-smoke
    COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/../tests/check_formatter.py
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_CURRENT_SOURCE_DIR}/../examples/example.txt
  )
endif()
