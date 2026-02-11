set(BLP ON CACHE BOOL "" FORCE)
set(MAP ON CACHE BOOL "" FORCE)
set(MPQ ON CACHE BOOL "" FORCE)
set(MDLX ON CACHE BOOL "" FORCE)
set(DOC OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/wc3lib SYSTEM)

set(WC3_BUILD_INCLUDES
    "${CMAKE_SOURCE_DIR}/thirdparty/wc3lib/src"
    "${CMAKE_SOURCE_DIR}/thirdparty/wc3lib/lib"
    "${CMAKE_BINARY_DIR}/thirdparty/wc3lib")

# suppress warnings for thirdparty libraries
set_property(TARGET wc3libcore PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET wc3libblp PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET wc3libmdlx PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET wc3libmap PROPERTY CXX_CLANG_TIDY "")
set_property(TARGET wc3libmpq PROPERTY CXX_CLANG_TIDY "")

target_compile_options(wc3libcore PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/WX->
)
target_compile_options(wc3libblp PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/WX->
)
target_compile_options(wc3libmdlx PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/WX->
)
target_compile_options(wc3libmap PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/WX->
)
target_compile_options(wc3libmpq PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/WX->
)
