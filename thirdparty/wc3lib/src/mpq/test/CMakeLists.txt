find_package(Catch2 REQUIRED CONFIG)

add_executable(archivetest archive.cpp)
target_link_libraries(archivetest PRIVATE wc3libmpq wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(archivetest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.mpq.ArchiveTest COMMAND archivetest)

add_executable(listfiletest listfile.cpp)
target_link_libraries(listfiletest PRIVATE wc3libmpq wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(listfiletest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.mpq.ListfileTest COMMAND listfiletest)

#smpq -SM 1 -c testattributes.mpq test.txt
add_executable(attributestest attributes.cpp)
target_link_libraries(attributestest PRIVATE wc3libmpq wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(attributestest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.mpq.AttributesTest COMMAND attributestest)

message(STATUS "Using Warcraft III directory: \"${WC3_DIR}\"")
if ("${WC3_DIR}" STREQUAL "")
	message(WARNING "Warcraft III directory is not defined. Some Unit Tests may not work.")
endif ()

add_definitions(-DWC3_DIR="\"${WC3_DIR}\"")

# TODO find StormLib with a CMake module
find_file(STORMLIB_HEADER "StormLib.h")
find_file(STORMLIB_PORT_HEADER "StormPort.h")

if (STORMLIB_HEADER AND STORMLIB_PORT_HEADER)
	add_executable(performancetest performance.cpp)
	target_link_libraries(performancetest PRIVATE wc3libmpq wc3libcore storm ${GETTEXT_LIBRARIES})
	target_link_libraries(performancetest PRIVATE Catch2::Catch2WithMain)
	add_test(NAME wc3lib.mpq.PerformanceTest COMMAND performancetest)
endif ()

add_executable(extracttest extract.cpp)
target_link_libraries(extracttest PRIVATE wc3libmpq wc3libcore ${GETTEXT_LIBRARIES})
	target_link_libraries(extracttest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.mpq.ExtractTest COMMAND extracttest)

configure_file(test_with_attributes_crc32.mpq test_with_attributes_crc32.mpq COPYONLY) # MPQ archive with extended attributes
configure_file(test_with_all_attributes.mpq test_with_all_attributes.mpq COPYONLY)
configure_file(test_with_no_extended_attributes.mpq test_with_no_extended_attributes.mpq COPYONLY)