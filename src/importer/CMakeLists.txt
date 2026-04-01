find_package(fmt CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(importer)

target_sources(importer
    PRIVATE
      FILE_SET CXX_MODULES
      FILES
        importer.cppm

    PRIVATE
      importer.cpp
      main.cpp
)

target_link_libraries(
  importer
  PRIVATE assets_mpq_importer::assets_mpq_importer_options
          assets_mpq_importer::assets_mpq_importer_warnings)

target_link_libraries(
  importer
  PRIVATE
          assets_mpq_importer::mpq_library
          assets_mpq_importer::blp_library
          assets_mpq_importer::mdlx_library
          assets_mpq_importer::w3m_library
)

target_link_system_libraries(
  importer
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog)

target_include_directories(importer PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

set_target_properties(importer PROPERTIES CXX_SCAN_FOR_MODULES ON)

file(GLOB SHARES_LIBS
  "${CMAKE_CURRENT_BINARY_DIR}/Release/*.dll"
  "${CMAKE_CURRENT_BINARY_DIR}/Release/*.so")
install(FILES ${SHARES_LIBS}
    DESTINATION bin
    CONFIGURATIONS Release
)
install(TARGETS importer
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
