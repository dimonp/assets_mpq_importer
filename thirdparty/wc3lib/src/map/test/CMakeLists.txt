find_package(Catch2 REQUIRED CONFIG)

add_executable(customobjectstest customobjects.cpp)
target_link_libraries(customobjectstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(customobjectstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.CustomObjectsTest COMMAND customobjectstest)

add_executable(customobjectstesttpof customobjectstpof.cpp)
target_link_libraries(customobjectstesttpof PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(customobjectstesttpof PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.CustomObjectsTestTPoF COMMAND customobjectstesttpof)

add_executable(customobjectstestwowr customobjectswowr.cpp)
target_link_libraries(customobjectstestwowr PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(customobjectstestwowr PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.CustomObjectsTestWoWR COMMAND customobjectstestwowr)

add_executable(txttest txt.cpp)
target_link_libraries(txttest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(txttest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.TxtTest COMMAND txttest)

add_executable(slktest slk.cpp)
target_link_libraries(slktest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(slktest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.SlkTest COMMAND slktest)

add_executable(triggerstest triggers.cpp)
target_link_libraries(triggerstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(triggerstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.TriggersTest COMMAND triggerstest)

add_executable(customtexttriggerstest customtexttriggers.cpp)
target_link_libraries(customtexttriggerstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(customtexttriggerstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.CustomTextTriggersTest COMMAND customtexttriggerstest)

add_executable(triggerstringstest triggerstrings.cpp)
target_link_libraries(triggerstringstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(triggerstringstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.TriggerStringsTest COMMAND triggerstringstest)

add_executable(rectstest rects.cpp)
target_link_libraries(rectstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(rectstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.RectsTest COMMAND rectstest)

add_executable(soundstest sounds.cpp)
target_link_libraries(soundstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(soundstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.SoundsTest COMMAND soundstest)

add_executable(unitsanditemstest unitsanditems.cpp)
target_link_libraries(unitsanditemstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(unitsanditemstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.UnitsAndItemsTest COMMAND unitsanditemstest)

configure_file(SkibisCastleTD/war3map.wts SkibisCastleTD/war3map.wts COPYONLY)
add_executable(mapstringstest mapstrings.cpp)
target_link_libraries(mapstringstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(mapstringstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.MapStringsTest COMMAND mapstringstest)
# The Power of Fire
configure_file(war3map_Talras_de.wts war3map_Talras_de.wts COPYONLY)
configure_file(war3map_Talras_en.wts war3map_Talras_en.wts COPYONLY)

if (MPQ)
add_executable(wc3maptest wc3map.cpp)
target_link_libraries(wc3maptest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(wc3maptest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.Wc3MapTest COMMAND wc3maptest)
endif ()

add_executable(infotest info.cpp)
target_link_libraries(infotest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(infotest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.InfoTest COMMAND infotest)

if (MPQ)
add_executable(warchaserstest warchasers.cpp)
target_link_libraries(warchaserstest PRIVATE wc3libmap wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(warchaserstest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.map.WarChasersTest COMMAND warchaserstest)
endif ()

configure_file(customobjects.w3o customobjects.w3o COPYONLY) # The Frozen Throne
configure_file(customobjectstpof.w3o customobjectstpof.w3o COPYONLY) # The Frozen Throne: The Power of Fire
configure_file(reforged/war3map.w3u reforged/war3map.w3u COPYONLY) # Reforged: World of Warcraft Reforged
configure_file(reforged/war3map.w3a reforged/war3map.w3a COPYONLY) # Reforged: World of Warcraft Reforged
configure_file(reforged/war3mapUnits.doo reforged/war3mapUnits.doo COPYONLY) # Reforged: World of Warcraft Reforged
configure_file(TriggerData.txt TriggerData.txt COPYONLY) # Warcraft III
configure_file(TriggerDataEx.txt TriggerDataEx.txt COPYONLY) # Warcraft III: The Frozen Throne
configure_file(TriggerStrings.txt TriggerStrings.txt COPYONLY) # Warcraft III
configure_file(WorldEditStrings.txt WorldEditStrings.txt COPYONLY) # The Frozen Throne "War3xlocal.mpq"
configure_file(WorldEditData.txt WorldEditData.txt COPYONLY) # The Frozen Throne "War3Patch.mpq"
configure_file(UnitAbilities.slk UnitAbilities.slk COPYONLY) # Reign of Chaos "war3.mpq"
configure_file(UnitBalance.slk UnitBalance.slk COPYONLY) # Reign of Chaos "war3.mpq"
configure_file(UnitData.slk UnitData.slk COPYONLY) # Reign of Chaos "war3.mpq"
configure_file(UnitMetaData.slk UnitMetaData.slk COPYONLY) # Reign of Chaos "war3.mpq"
configure_file(UnitMetaDataEx.slk UnitMetaDataEx.slk COPYONLY) # Frozen Throne "war3x.mpq"
configure_file(UnitMetaDataExportedByLibreOffice.slk UnitMetaDataExportedByLibreOffice.slk COPYONLY) # Reign of Chaos "war3.mpq" exported with LibreOffice
configure_file(UnitUI.slk UnitUI.slk COPYONLY) # Reign of Chaos "war3.mpq"
configure_file(UnitWeapons.slk UnitWeapons.slk COPYONLY) # Reign of Chaos "war3.mpq"
configure_file(UpgradeData.slk UpgradeData.slk COPYONLY) # Reign of Chaos "war3.mpq"
configure_file(ItemFunc.txt ItemFunc.txt COPYONLY) # Frozen Throne "War3Patch.mpq"
configure_file(ItemStrings.txt ItemStrings.txt COPYONLY) # Frozen Throne "War3Patch.mpq"
configure_file(war3map.wtg war3map.wtg COPYONLY) # War Chasers
configure_file(war3map.wct war3map.wct COPYONLY) # War Chasers
configure_file(TestMap.w3m TestMap.w3m COPYONLY)
configure_file(war3map.w3r war3map.w3r COPYONLY) # Reign of Chaos rects
configure_file(war3map.w3s war3map.w3s COPYONLY) # Reign of Chaos sounds
configure_file(war3map.w3i war3map.w3i COPYONLY) # Reign of Chaos War Chasers
configure_file(war3map.wts war3map.wts COPYONLY) # Reign of Chaos War Chasers
configure_file(UnitEditorData.txt UnitEditorData.txt COPYONLY) # Reign of Chaos (war3.mpq)
configure_file(NeutralUnitFunc.txt NeutralUnitFunc.txt COPYONLY) # Reign of Chaos (war3.mpq)

configure_file(\(4\)WarChasers.w3m \(4\)WarChasers.w3m COPYONLY)
