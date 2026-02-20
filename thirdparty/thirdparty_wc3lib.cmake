set(BLP ON CACHE BOOL "" FORCE)
set(MAP ON CACHE BOOL "" FORCE)
set(MPQ ON CACHE BOOL "" FORCE)
set(MDLX ON CACHE BOOL "" FORCE)
set(DOC OFF CACHE BOOL "" FORCE)

add_subdirectory(wc3lib SYSTEM)

set(WC3_LIBRARY_INCLUDES
    "${CMAKE_SOURCE_DIR}/thirdparty/wc3lib/src"
    "${CMAKE_SOURCE_DIR}/thirdparty/wc3lib/lib"
    "${CMAKE_BINARY_DIR}/thirdparty/wc3lib"
    PARENT_SCOPE
)
