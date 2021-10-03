##########################################
## main library ##
##########################################

# /W3 warning in msvc fixed with 3.15
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(
    sqlite-cpm
    VERSION 0.1.0
    LANGUAGES C CXX
    DESCRIPTION "SQLite with CPM.cmake integration"
)

# project options
option(SQLITECPM_BUILD_EXAMPLE "Enable building the example" OFF)

# do not allow building in-source
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# dependencies
include(cmake/CPM.cmake)
# PackageProject.cmake will be used to make our project installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")


# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.c")

# base library
set(PROJECT_NAMESPACE ${PROJECT_NAME})
add_library(${PROJECT_NAME} ${headers} ${sources})
message(STATUS "check headers: ${headers}, sources: ${sources}")
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(SQLITECPM_BUILD_EXAMPLE)
    message(STATUS "Building sample enabled")
    add_subdirectory(example)
else()
    message(STATUS "Building sample DISABLED")
endif()

# make installable target
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)
packageProject(
    NAME ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    NAMESPACE ${PROJECT_NAMESPACE}
    BINARY_DIR ${PROJECT_BINARY_DIR}
    INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
    INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
    VERSION_HEADER "${VERSION_HEADER_LOCATION}"
    DEPENDENCIES ""
)