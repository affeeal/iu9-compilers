cmake_minimum_required(VERSION 3.22.1)
project(generator LANGUAGES CXX)

find_package(FLEX REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

flex_target(scanner lexer/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)

add_executable(${PROJECT_NAME} main.cc ${FLEX_scanner_OUTPUTS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(semantics)

include(CTest)
if(BUILD_TESTING)
  find_package(Python3 COMPONENTS Interpreter REQUIRED)

  # Resolve paths to ensure they're absolute and correct
  file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../tests/check_generator.py"
       CHECK_GENERATOR_SCRIPT)
  file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../templates/analyzer_table.cc"
       TEMPLATE_FILE)
  file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../examples/arithmetic_grammar.txt"
       ARITHMETIC_GRAMMAR)
  file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../examples/self-describing_grammar.txt"
       SELF_GRAMMAR)
  file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/parser/analyzer_table.cc"
       CHECKED_TABLE)
  file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../apps/calculator/src/parser"
       CALCULATOR_INCLUDE)
  file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/parser"
       GENERATOR_INCLUDE)

  add_test(
    NAME generator-regression
    COMMAND ${Python3_EXECUTABLE}
            ${CHECK_GENERATOR_SCRIPT}
            $<TARGET_FILE:${PROJECT_NAME}>
            ${TEMPLATE_FILE}
            ${ARITHMETIC_GRAMMAR}
            ${SELF_GRAMMAR}
            ${CHECKED_TABLE}
            ${CALCULATOR_INCLUDE}
            ${GENERATOR_INCLUDE}
            ${CMAKE_CXX_COMPILER}
  )
endif()
