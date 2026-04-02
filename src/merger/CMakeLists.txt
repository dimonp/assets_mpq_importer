find_package(fmt CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(merger)

target_sources(merger
    PRIVATE
      FILE_SET CXX_MODULES
      FILES
        merger.cppm

    PRIVATE
      wavefront_obj.cpp
      transformations.cpp
      w3map.cpp
      main.cpp
)

target_link_libraries(merger
  PRIVATE assets_mpq_importer::assets_mpq_importer_options
          assets_mpq_importer::assets_mpq_importer_warnings)

target_link_system_libraries(merger
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog
          Microsoft.GSL::GSL
          assimp::assimp
          nlohmann_json::nlohmann_json)

target_include_directories(merger PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

set_target_properties(merger PROPERTIES CXX_SCAN_FOR_MODULES ON)

file(GLOB SHARES_LIBS
  "${CMAKE_CURRENT_BINARY_DIR}/Release/*.dll"
  "${CMAKE_CURRENT_BINARY_DIR}/Release/*.so")
install(FILES ${SHARES_LIBS}
  DESTINATION bin
  CONFIGURATIONS Release
)
install(TARGETS merger
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
