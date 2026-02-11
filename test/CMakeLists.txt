cmake_minimum_required(VERSION 3.15...3.23)

project(CmakeConfigPackageTests LANGUAGES CXX)

# ---- Test as standalone project the exported config package ----

if(PROJECT_IS_TOP_LEVEL OR TEST_INSTALLED_VERSION)
  enable_testing()

  find_package(assets_mpq_importer CONFIG REQUIRED) # for intro, project_options, ...

  if(NOT TARGET assets_mpq_importer_options)
    message(FATAL_ERROR "Required config package not found!")
    return() # be strictly paranoid for Template Janitor github action! CK
  endif()
endif()

find_package(Catch2 CONFIG REQUIRED)
include(Catch)

# Provide a simple smoke test to make sure that the CLI works and can display a --help message
add_test(NAME cli.importer.has_help COMMAND importer --help)
add_test(NAME cli.merger.has_help COMMAND merger --help)

# Provide a test to verify that the version being reported from the application
# matches the version given to CMake. This will be important once you package
# your program. Real world shows that this is the kind of simple mistake that is easy
# to make, but also easy to test for.
add_test(NAME cli.importer.version_matches COMMAND importer --version)
set_tests_properties(cli.importer.version_matches PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

add_test(NAME cli.merger.version_matches COMMAND merger --version)
set_tests_properties(cli.merger.version_matches PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testdata DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(tests test_utils.hpp mpq_library_tests.cpp blp_library_tests.cpp mdlx_library_tests.cpp w3m_library_tests.cpp)
target_link_libraries(
  tests
  PRIVATE assets_mpq_importer::assets_mpq_importer_warnings
          assets_mpq_importer::assets_mpq_importer_options
          Catch2::Catch2WithMain)

target_link_libraries(
  tests
  PRIVATE
          assets_mpq_importer::mpq_library
          assets_mpq_importer::blp_library
          assets_mpq_importer::mdlx_library
          assets_mpq_importer::w3m_library)


target_include_directories(
    tests SYSTEM PRIVATE
      ${WC3_BUILD_INCLUDES}
      ${NVTT_BUILD_INCLUDES})

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET tests
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:tests> $<TARGET_FILE_DIR:tests>
    COMMAND_EXPAND_LISTS)
endif()

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  tests
  TEST_PREFIX "unittests."
  REPORTER XML
  OUTPUT_DIR .
  OUTPUT_PREFIX "unittests."
  OUTPUT_SUFFIX .xml)

