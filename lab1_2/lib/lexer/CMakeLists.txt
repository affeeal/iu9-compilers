set(MODULE_NAME lexer)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_library(${MODULE_NAME} ${SOURCES})
target_compile_features(${MODULE_NAME} PRIVATE cxx_std_20)
target_include_directories(${MODULE_NAME} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${MODULE_NAME} PRIVATE ${Boost_REGEX_LIBRARY})
