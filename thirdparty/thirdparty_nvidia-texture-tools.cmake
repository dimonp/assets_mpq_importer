if(POLICY CMP0069)
  cmake_policy(SET CMP0069 NEW)
endif()

if(POLICY CMP0146)
  cmake_policy(SET CMP0146 OLD)
endif()

add_subdirectory(thirdparty/nvidia-texture-tools SYSTEM)

set(NVTT_BUILD_INCLUDES
    "${NV_SOURCE_DIR}/src"
    "${NV_SOURCE_DIR}/extern"
    "${NV_SOURCE_DIR}/extern/poshlib"
)

# suppress warnings for thirdparty library
set_property(TARGET nvtt PROPERTY CXX_CLANG_TIDY "")
target_compile_options(nvtt PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/W0 /WX->
)
