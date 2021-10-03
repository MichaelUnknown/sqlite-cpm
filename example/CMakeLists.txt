##########################################
## exmaples ##
##########################################

# /W3 warning in msvc fixed with 3.15
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

CPMAddPackage("gh:fmtlib/fmt#8.0.1")

add_executable(demo_sqlite demo_sqlite.cpp)
target_link_libraries(demo_sqlite PRIVATE fmt::fmt ${PROJECT_NAMESPACE}::${PROJECT_NAME})
