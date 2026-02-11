find_package(fmt CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(merger)

target_sources(merger
    PRIVATE
      main.cpp
      wavefront_obj.cpp
      transformations.cpp
      w3map.cpp
    PRIVATE
      FILE_SET HEADERS
      FILES
        merger.hpp
)


target_link_libraries(
  merger
  PRIVATE assets_mpq_importer::assets_mpq_importer_options
          assets_mpq_importer::assets_mpq_importer_warnings)

target_link_system_libraries(
  merger
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog
          Microsoft.GSL::GSL
          assimp::assimp
          nlohmann_json::nlohmann_json)

target_include_directories(merger PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

