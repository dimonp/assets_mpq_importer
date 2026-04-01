add_library(common)
add_library(assets_mpq_importer::common ALIAS common)

target_sources(common
    PUBLIC
      FILE_SET CXX_MODULES
      FILES
        assmpq.cppm
)

set_target_properties(common PROPERTIES CXX_SCAN_FOR_MODULES ON)