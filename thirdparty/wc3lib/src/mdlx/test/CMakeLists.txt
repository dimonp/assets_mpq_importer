find_package(Catch2 REQUIRED CONFIG)

add_executable(mdlxfiletest mdlxfiletest.cpp)
target_link_libraries(mdlxfiletest PRIVATE wc3libmdlx wc3libcore ${GETTEXT_LIBRARIES})
target_link_libraries(mdlxfiletest PRIVATE Catch2::Catch2WithMain)
add_test(NAME wc3lib.mdlx.MdlxFileTest COMMAND mdlxfiletest)

# MDL files are converted by MdlxConv
configure_file(Alliance_Exp.mdl Alliance_Exp.mdl COPYONLY)
configure_file(Alliance_Exp.mdx Alliance_Exp.mdx COPYONLY)
configure_file(NightElf_Exp.mdl NightElf_Exp.mdl COPYONLY)
configure_file(NightElf_Exp.mdx NightElf_Exp.mdx COPYONLY)
configure_file(Orc_Exp.mdl Orc_Exp.mdl COPYONLY)
configure_file(Orc_Exp.mdx Orc_Exp.mdx COPYONLY)
configure_file(Undead3D_Exp.mdl Undead3D_Exp.mdl COPYONLY)
configure_file(Undead3D_Exp.mdx Undead3D_Exp.mdx COPYONLY)
