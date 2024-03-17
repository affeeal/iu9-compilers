cmake_minimum_required(VERSION 3.22.1)
project(lab1-5)

find_package(FLEX REQUIRED)

flex_target(scanner
  scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp
)

add_executable(${PROJECT_NAME}
  main.cpp
  ${FLEX_scanner_OUTPUTS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
