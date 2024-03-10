set(MODULE_NAME lexer)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_library(${MODULE_NAME} ${SOURCES})
target_include_directories(${MODULE_NAME} PRIVATE ${INCLUDE_DIR})
