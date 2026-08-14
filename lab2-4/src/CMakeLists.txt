cmake_minimum_required(VERSION 3.22.1)
project(lab2-4 LANGUAGES CXX)

find_package(Boost 1.75 COMPONENTS json REQUIRED)
find_package(FLEX REQUIRED)

flex_target(scanner
  scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc
)

add_executable(${PROJECT_NAME}
  compiler.cc
  fragment.cc
  main.cc
  message.cc
  node.cc
  parser.cc
  position.cc
  token.cc
  ${FLEX_scanner_OUTPUTS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::json)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(CTest)
if(BUILD_TESTING)
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  add_test(
    NAME lab2-4-smoke
    COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/../tests/check_parser.py
            $<TARGET_FILE:${PROJECT_NAME}>
            ${CMAKE_CURRENT_SOURCE_DIR}/../examples/program.txt
  )
endif()
