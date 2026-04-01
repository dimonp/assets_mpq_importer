find_package(assimp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

include(GenerateExportHeader)

add_library(mdlx_library OBJECT)
add_library(assets_mpq_importer::mdlx_library ALIAS mdlx_library)

target_sources(mdlx_library
    PUBLIC
      FILE_SET CXX_MODULES
      FILES
        mdlx.cppm

    PRIVATE
      converter_obj.cpp
)

target_link_libraries(mdlx_library
  PRIVATE
    assets_mpq_importer_options assets_mpq_importer_warnings
    assets_mpq_importer::common
    wc3libcore
    wc3libmdlx)

target_link_system_libraries(
  mdlx_library
  PRIVATE
          assimp::assimp
          spdlog::spdlog)


target_include_directories(mdlx_library
  ${WARNING_GUARD} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_include_directories(mdlx_library
  SYSTEM PRIVATE
    "\$<BUILD_INTERFACE:${WC3_LIBRARY_INCLUDES}>")

if (assets_mpq_importer_BUILD_FUZZ_TESTS)
  target_link_libraries(mdlx_library PRIVATE -fsanitize=fuzzer-no-link)
  target_compile_options(mdlx_library PRIVATE -fsanitize=fuzzer-no-link)
endif()

target_compile_features(mdlx_library PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

set_target_properties(mdlx_library PROPERTIES CXX_SCAN_FOR_MODULES ON)
set_target_properties(
  mdlx_library
  PROPERTIES VERSION ${PROJECT_VERSION}
              C_VISIBILITY_PRESET hidden
              CXX_VISIBILITY_PRESET hidden
              VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(mdlx_library EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/assets_mpq_importer/mdlx_library_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(mdlx_library PUBLIC mdlx_library_STATIC_DEFINE)
endif()
