find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(blp_library OBJECT)
add_library(assets_mpq_importer::blp_library ALIAS blp_library)

target_sources(blp_library
  PUBLIC
    FILE_SET CXX_MODULES
    FILES
      blp.cppm

  PRIVATE
      utils_blp.cpp utils_blp.hpp
      converter_png.cpp
      converter_dds_nvtt.cpp
      converter_dds_amdc.cpp
)

target_link_libraries(
  blp_library PRIVATE
          assets_mpq_importer::assets_mpq_importer_options
          assets_mpq_importer::assets_mpq_importer_warnings
          assets_mpq_importer::common)

target_link_system_libraries(
  blp_library
  PRIVATE
          nvtt
          CMP_Compressonator
          CMP_Framework
          wc3libcore
          wc3libblp)


target_include_directories(blp_library
  ${WARNING_GUARD} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_include_directories(blp_library
  SYSTEM PRIVATE
    "\$<BUILD_INTERFACE:${WC3_LIBRARY_INCLUDES}>"
    "\$<BUILD_INTERFACE:${NVTT_LIBRARY_INCLUDES}>"
    "\$<BUILD_INTERFACE:${AMDC_LIBRARY_INCLUDES}>")

if (assets_mpq_importer_BUILD_FUZZ_TESTS)
  target_link_libraries(blp_library PRIVATE -fsanitize=fuzzer-no-link)
  target_compile_options(blp_library PRIVATE -fsanitize=fuzzer-no-link)
endif()

target_compile_features(blp_library PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

set_target_properties(blp_library PROPERTIES CXX_SCAN_FOR_MODULES ON)
set_target_properties(
  blp_library
  PROPERTIES VERSION ${PROJECT_VERSION}
              C_VISIBILITY_PRESET hidden
              CXX_VISIBILITY_PRESET hidden
              VISIBILITY_INLINES_HIDDEN YES)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(blp_library PUBLIC blp_library_STATIC_DEFINE)
endif()
