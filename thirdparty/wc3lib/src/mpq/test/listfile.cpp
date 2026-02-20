/***************************************************************************
 *   Copyright (C) 2016 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "../listfile.hpp"
#include "../archive.hpp"

using namespace wc3lib;

/*
 2.7 LISTFILE
 The listfile is a very simple extension to the MoPaQ format that contains the file paths of (most) files in the archive. The languages and platforms of the files are not stored in the listfile. The listfile is contained in the file "(listfile)" (default language and platform), and is simply a text file with file paths separated by ';', 0Dh, 0Ah, or some combination of these. The file "(listfile)" may not be listed in the listfile.
 */
TEST_CASE("ListfileEntriesTest")
{
	std::stringstream sstream;
	sstream <<
		"bla1;"
		"bla2\n"
		"bla3\r"
		// ;
		"bla4;\n\r"
		"bla5;\r\n"
		"bla6;\n"
		"bla7;\r"
		// line break
		"bla8\n\r;"
		"bla9\n;\r"
		"bla10\n;"
		"bla11\n\r"
		// carriage return
		"bla12\r\n;"
		"bla13\r;\n"
		"bla14\r\n"
		"bla15\r;"
		"end"
	;

	mpq::Listfile::Entries entries = mpq::Listfile::entries(sstream.str());

	REQUIRE(entries.size() == 16);
	REQUIRE(entries[0] == "bla1");
	REQUIRE(entries[1] == "bla2");
	REQUIRE(entries[2] == "bla3");
	REQUIRE(entries[3] == "bla4");
	REQUIRE(entries[4] == "bla5");
	REQUIRE(entries[5] == "bla6");
	REQUIRE(entries[6] == "bla7");
	REQUIRE(entries[7] == "bla8");
	REQUIRE(entries[8] == "bla9");
	REQUIRE(entries[9] == "bla10");
	REQUIRE(entries[10] == "bla11");
	REQUIRE(entries[11] == "bla12");
	REQUIRE(entries[12] == "bla13");
	REQUIRE(entries[13] == "bla14");
	REQUIRE(entries[14] == "bla15");
	REQUIRE(entries.back() == "end");
}

TEST_CASE("ListfileContentTest")
{
	mpq::Listfile::Entries entries;
	entries.push_back("bla1");
	entries.push_back("bla2");
	entries.push_back("bla3");
	entries.push_back("bla4");
	entries.push_back("bla5");
	entries.push_back("bla6");
	entries.push_back("bla7");
	entries.push_back("bla8");
	entries.push_back("bla9");
	entries.push_back("bla10");
	entries.push_back("bla11");
	entries.push_back("bla12");
	entries.push_back("bla13");
	entries.push_back("bla14");
	entries.push_back("bla15");
	entries.push_back("end");

	const std::string content = mpq::Listfile::content(entries);

	entries = mpq::Listfile::entries(content);
	REQUIRE(entries.size() == 16);
	REQUIRE(entries[0] == "bla1");
	REQUIRE(entries[1] == "bla2");
	REQUIRE(entries[2] == "bla3");
	REQUIRE(entries[3] == "bla4");
	REQUIRE(entries[4] == "bla5");
	REQUIRE(entries[5] == "bla6");
	REQUIRE(entries[6] == "bla7");
	REQUIRE(entries[7] == "bla8");
	REQUIRE(entries[8] == "bla9");
	REQUIRE(entries[9] == "bla10");
	REQUIRE(entries[10] == "bla11");
	REQUIRE(entries[11] == "bla12");
	REQUIRE(entries[12] == "bla13");
	REQUIRE(entries[13] == "bla14");
	REQUIRE(entries[14] == "bla15");
	REQUIRE(entries.back() == "end");
}

TEST_CASE("CaseFileSensitiveEntries")
{
	std::stringstream sstream;
	sstream <<
		"Abilities"
		";abilities"
		";abilities\\Hans" // must be excluded since it is not recursive
	;

	const mpq::Listfile::Entries entries = mpq::Listfile::entries(sstream.str());

	REQUIRE(entries.size() == 3);

	const mpq::Listfile::Entries uniqueEntries = mpq::Listfile::caseSensitiveFileEntries(entries, "", false);

	/*
	for (std::size_t i = 0; i < uniqueEntries.size(); ++i)
	{
		std::cerr << uniqueEntries[i] << std::endl;
	}
	*/

	REQUIRE(uniqueEntries.size() == 2);
	REQUIRE(uniqueEntries[0] == "Abilities");
	REQUIRE(uniqueEntries[1] == "abilities");
}

TEST_CASE("CaseSensitiveFileEntriesRecursive")
{
	std::stringstream sstream;
	sstream <<
		"Abilities\\Hans"
		";abilities\\Peter"
		";abilities\\PeTeR"
		";abILIties\\UI\\test"
		";abilities\\ui\\test2"
	;

	const mpq::Listfile::Entries entries = mpq::Listfile::entries(sstream.str());

	REQUIRE(entries.size() == 5);

	const mpq::Listfile::Entries uniqueEntries = mpq::Listfile::caseSensitiveFileEntries(entries, "", true);

	/*
	BOOST_FOREACH(mpq::Listfile::Entries::const_reference ref, uniqueEntries)
	{
		std::cerr << ref << std::endl;
	}
	*/

	REQUIRE(uniqueEntries.size() == 5);
	REQUIRE(uniqueEntries[0] == "Abilities\\Hans");
	REQUIRE(uniqueEntries[1] == "Abilities\\Peter");
	REQUIRE(uniqueEntries[2] == "Abilities\\Peter");
	REQUIRE(uniqueEntries[3] == "Abilities\\UI\\test");
	REQUIRE(uniqueEntries[4] == "Abilities\\UI\\test2");
}

TEST_CASE("CaseSensitiveFileEntriesRecursiveExtension")
{
	std::stringstream sstream;
	sstream <<
		"Abilities\\Hans"
		";abilities\\Peter"
		";abilities\\PeTeR"
		";abILIties\\UI\\test"
		";abilities\\ui\\test2"
		";abilities\\ui\\test3.txt"
		;

	const mpq::Listfile::Entries entries = mpq::Listfile::entries(sstream.str());

	REQUIRE(entries.size() == 6);

	const mpq::Listfile::Entries uniqueEntries = mpq::Listfile::caseSensitiveFileEntries(entries, "", true);

	/*
	BOOST_FOREACH(mpq::Listfile::Entries::const_reference ref, uniqueEntries)
	{
		std::cerr << ref << std::endl;
	}
	*/

	REQUIRE(uniqueEntries.size() == 6);
	REQUIRE(uniqueEntries[0] == "Abilities\\Hans");
	REQUIRE(uniqueEntries[1] == "Abilities\\Peter");
	REQUIRE(uniqueEntries[2] == "Abilities\\Peter");
	REQUIRE(uniqueEntries[3] == "Abilities\\UI\\test");
	REQUIRE(uniqueEntries[4] == "Abilities\\UI\\test2");
	REQUIRE(uniqueEntries[5] == "Abilities\\UI\\test3.txt");
}

TEST_CASE("CaseSensitiveDirEntriesRecursive")
{
	std::stringstream sstream;
	sstream <<
		"Abilities\\Hans\\bla"
		";abilities\\Peter\\blu"
		";abilities\\Peter\\bla"
		";abilities\\PeTeR\\bli"
		";abILIties\\UI\\test"
		";abilities\\ui\\test2"
		";abilities\\ui\\test3.txt"
	;

	const mpq::Listfile::Entries entries = mpq::Listfile::entries(sstream.str());

	REQUIRE(entries.size() == 7);

	const mpq::Listfile::Entries uniqueEntries = mpq::Listfile::caseSensitiveDirEntries(entries, "", true);

	/*
	BOOST_FOREACH(mpq::Listfile::Entries::const_reference ref, uniqueEntries)
	{
		std::cerr << ref << std::endl;
	}
	*/

	REQUIRE(uniqueEntries.size() == 4);
	REQUIRE(uniqueEntries[0] == "Abilities");
	REQUIRE(uniqueEntries[1] == "Abilities\\Hans");
	REQUIRE(uniqueEntries[2] == "Abilities\\Peter");
	REQUIRE(uniqueEntries[3] == "Abilities\\UI");
}

/*
 * Tests listing directory dir entries with the prefix "Abilities\\" but without recursion.
 * Therefore the directory "testDir" should not be listed at all.
 */
TEST_CASE("CaseSensitiveDirEntriesWithPrefix")
{
	std::stringstream sstream;
	sstream <<
		"Abilities\\Hans\\bla"
		";abilities\\Peter\\blu"
		";abilities\\PeTeR\\bli"
		";abILIties\\UI\\test"
		";abILIties\\UI\\test1"
		";abilities\\ui\\test2"
		";abilities\\ui\\test3.txt"
	/*
	 * This is a recursive sub directory called "testDir" which should not be listed since recursive is "false".
	 */
		";abilities\\ui\\testDir\\test"
		";test\\ui\\test3.txt"
		";test\\ui\\anothersubdir\\myfile.txt"
	;

	const mpq::Listfile::Entries entries = mpq::Listfile::entries(sstream.str());

	REQUIRE(entries.size() == 10);

	const mpq::Listfile::Entries uniqueEntries = mpq::Listfile::caseSensitiveDirEntries(entries, "Abilities\\", false);

	/*
	BOOST_FOREACH(mpq::Listfile::Entries::const_reference ref, uniqueEntries)
	{
		std::cerr << ref << std::endl;
	}
	*/

	REQUIRE(uniqueEntries.size() == 3);
	REQUIRE(uniqueEntries[0] == "Abilities\\Hans");
	REQUIRE(uniqueEntries[1] == "Abilities\\Peter");
	REQUIRE(uniqueEntries[2] == "Abilities\\UI");
}

/*
 * Tests if all directory entries are detected without recursion and prefix.
 * If no recursion and no prefix is used it must list all directories in the top level directory.
 */
TEST_CASE("CaseSensitiveDirEntriesWithEmptyPrefix")
{
	std::stringstream sstream;
	sstream <<
		"Abilities\\Hans\\bla"
		";abilities\\Peter\\blu"
		";abilities\\PeTeR\\bli"
		";abILIties\\UI\\test"
		";abILIties\\UI\\test1"
		";abilities\\ui\\test2"
		";abilities\\ui\\test3.txt"
		";test\\ui\\test3.txt"
		";test\\ui\\anothersubdir\\myfile.txt"
	;

	const mpq::Listfile::Entries entries = mpq::Listfile::entries(sstream.str());

	REQUIRE(entries.size() == 9);

	const mpq::Listfile::Entries uniqueEntries = mpq::Listfile::caseSensitiveDirEntries(entries, "", false);

	/*
	BOOST_FOREACH(mpq::Listfile::Entries::const_reference ref, uniqueEntries)
	{
		std::cerr << ref << std::endl;
	}
	*/

	REQUIRE(uniqueEntries.size() == 2);
	REQUIRE(uniqueEntries[0] == "Abilities");
	REQUIRE(uniqueEntries[1] == "test");
}

TEST_CASE("ExistingEntriesWithPrefix")
{
	mpq::Archive archive;
	bool success = true;

	try
	{
		archive.open("test_with_no_extended_attributes.mpq");
	}
	catch (Exception &e)
	{
		std::cerr << e.what() << std::endl;

		success = false;
	}

	REQUIRE(success);
	std::stringstream sstream;
	sstream <<
		"test\\testfile.txt"
		";testfile.txt"
	;

	const mpq::Listfile::Entries listfileEntries = mpq::Listfile::entries(sstream.str());

	REQUIRE(listfileEntries.size() == 2);

	const mpq::Listfile::Entries entries = mpq::Listfile::existingEntries(listfileEntries, archive, "test\\", true);

	/*
	BOOST_FOREACH(mpq::Listfile::Entries::const_reference ref, entries)
	{
		std::cerr << ref << std::endl;
	}
	*/

	REQUIRE(entries.size() == 1);

	REQUIRE(entries[0] == "test\\testfile.txt");
}

TEST_CASE("DirPath")
{
	CHECK(mpq::Listfile::dirPath("") == "");
	CHECK(mpq::Listfile::dirPath("Bla") == "");
	CHECK(mpq::Listfile::dirPath("Bla\\bla") == "Bla");
	CHECK(mpq::Listfile::dirPath("Bla\\bla\\bla.txt") == "Bla\\bla");
}

TEST_CASE("FileName")
{
	CHECK(mpq::Listfile::fileName("") == "");
	const string filePath0 = "test.txt";
	CHECK(mpq::Listfile::fileName(filePath0) == "test.txt");
	const string filePath1 = "UI\\peter\\test.txt";
	CHECK(mpq::Listfile::fileName(filePath1) == "test.txt");
	const string filePath2 = "UI\\peter\\";
	CHECK(mpq::Listfile::fileName(filePath2) == "");
}
