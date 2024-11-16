file(GLOB_RECURSE sources LIST_DIRECTORIES false CONFIGURE_DEPENDS "*.cpp")

if(NOT "${sources}" STREQUAL "")
  if(NOT TARGET ktest)
    include(FetchContent)
    FetchContent_Declare(ktest
      GIT_REPOSITORY https://github.com/karnkaul/ktest
      GIT_TAG v0.1.0
      GIT_SHALLOW TRUE
      SOURCE_DIR "${ext_dir}/src/ktest"
    )
    FetchContent_MakeAvailable(ktest)
  endif()

  add_executable(${PROJECT_NAME}-tests)
  target_link_libraries(${PROJECT_NAME}-tests PRIVATE
    ktest::ktest-main
    ${PROJECT_NAME}::${PROJECT_NAME}
  )
  target_include_directories(${PROJECT_NAME}-tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../lib/src"
    "${CMAKE_CURRENT_SOURCE_DIR}"
  )
  target_sources(${PROJECT_NAME}-tests PRIVATE ${sources})
  add_test(NAME ${PROJECT_NAME}-tests COMMAND ${PROJECT_NAME}-tests)
endif()
