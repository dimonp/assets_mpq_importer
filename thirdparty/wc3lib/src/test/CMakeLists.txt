find_package(Catch2 REQUIRED CONFIG)

add_executable(readtest read.cpp)
target_link_libraries(readtest PRIVATE wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(readtest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.core.ReadTest COMMAND readtest)

add_executable(vertextest vertex.cpp)
target_link_libraries(vertextest PRIVATE wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(vertextest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.core.VertexTest COMMAND vertextest)
