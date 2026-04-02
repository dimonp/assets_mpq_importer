find_package(re2 CONFIG REQUIRED)

add_library(mpq_library OBJECT)
add_library(assets_mpq_importer::mpq_library ALIAS mpq_library)

target_sources(mpq_library
    PUBLIC
      FILE_SET CXX_MODULES
      FILES
        mpq.cppm

    PRIVATE
      mpq.cpp
)

target_link_libraries(mpq_library
  PRIVATE
          assets_mpq_importer::assets_mpq_importer_options 
          assets_mpq_importer::assets_mpq_importer_warnings
          assets_mpq_importer::common)

target_link_system_libraries(mpq_library
  PRIVATE
          wc3libcore
          wc3libmpq
          re2::re2)


target_include_directories(mpq_library
  ${WARNING_GUARD} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_include_directories(mpq_library SYSTEM PRIVATE "\$<BUILD_INTERFACE:${WC3_LIBRARY_INCLUDES}>")

target_compile_features(mpq_library PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

set_target_properties(mpq_library PROPERTIES CXX_SCAN_FOR_MODULES ON)
set_target_properties(
  mpq_library
  PROPERTIES VERSION ${PROJECT_VERSION}
              C_VISIBILITY_PRESET hidden
              CXX_VISIBILITY_PRESET hidden
              VISIBILITY_INLINES_HIDDEN YES)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(mpq_library PUBLIC mpq_library_STATIC_DEFINE)
endif()
