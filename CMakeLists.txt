cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(strings VERSION 1.1.0)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
include(file_list)
include(cmake/targets)
add_shared_lib("strings" "${SOURCES}" "${INCLUDE_DIRS}" "${PUBLIC_HEADERS}" "strings")

if(BUILD_TESTS)
  include(cmake/test)
endif()

unset(DEBUG_SCOPES CACHE)
