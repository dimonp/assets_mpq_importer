if(POLICY CMP0069)
  cmake_policy(SET CMP0069 NEW)
endif()

if(POLICY CMP0146)
  cmake_policy(SET CMP0146 OLD)
endif()

add_subdirectory(nvidia-texture-tools SYSTEM)

set(NVTT_LIBRARY_INCLUDES
    "${NV_SOURCE_DIR}/src"
    "${NV_SOURCE_DIR}/extern"
    "${NV_SOURCE_DIR}/extern/poshlib"
    PARENT_SCOPE
)
