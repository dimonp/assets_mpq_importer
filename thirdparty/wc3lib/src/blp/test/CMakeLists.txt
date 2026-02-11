find_package(Catch2 REQUIRED CONFIG)

add_executable(teamcolors teamcolors.cpp)
target_link_libraries(teamcolors PRIVATE wc3libblp wc3libcore)
target_link_libraries(teamcolors PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.blp.TeamColorsTest COMMAND teamcolors)

add_executable(jpeg jpeg.cpp)
target_link_libraries(jpeg PRIVATE wc3libblp wc3libcore)
target_link_libraries(jpeg PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.blp.JpegTest COMMAND jpeg)

add_executable(paletted paletted.cpp)
target_link_libraries(paletted PRIVATE wc3libblp wc3libcore)
target_link_libraries(paletted PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.blp.Paletted COMMAND paletted)

configure_file(TEAMCOLOR/TeamColor00.blp TeamColor00.blp COPYONLY) # Warcraft III: Reign of Chaos
# BLP 1 paletted + alpha
configure_file(HumanCampaignCastle.blp HumanCampaignCastle.blp COPYONLY) # Warcraft III: Reign of Chaos
