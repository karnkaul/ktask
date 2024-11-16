cmake_minimum_required(VERSION 3.23)

project(ktask VERSION "0.1.1")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "-d")

option(KTASK_BUILD_TESTS "Build ktask tests" ${PROJECT_IS_TOP_LEVEL})

add_library(${PROJECT_NAME}-compile-options INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME}-compile-options ALIAS ${PROJECT_NAME}-compile-options)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME}-compile-options INTERFACE
    -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
  )
endif()

add_subdirectory(lib)

if(KTASK_BUILD_TESTS)
  enable_testing()

  if("${ext_dir}" STREQUAL "")
    set(ext_dir "${CMAKE_CURRENT_SOURCE_DIR}/ext")
  endif()

  add_subdirectory(tests)
endif()
