target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(${PROJECT_NAME} PRIVATE
  ast.cc
  dt_to_ast.cc
  first_follow.cc
  analyzer_table_generator.cc
  analyzer_table.cc
)
