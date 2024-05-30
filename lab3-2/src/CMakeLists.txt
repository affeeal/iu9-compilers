cmake_minimum_required(VERSION 3.22.1)
project(lab3-2)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

flex_target(scanner
  scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc
)

bison_target(parser
  parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/parser.h")

add_flex_bison_dependency(scanner parser)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(${PROJECT_NAME} 
  ${SOURCES}
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
