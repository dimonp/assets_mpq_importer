/***************************************************************************
 *   Copyright (C) 2013 by Tamino Dauth                                    *
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
#include <fstream>
#include <sstream>
#include <chrono>
#include <catch2/catch_test_macros.hpp>

#include "../../platform.hpp"
#include "../txt.hpp"

using namespace wc3lib;

bool isHello(const map::Txt::Entry &entry)
{
	return entry.first == "Hello";
}

bool isHaha(const map::Txt::Entry &entry)
{
	return entry.first == "Haha";
}

TEST_CASE("TxtSimpleReadTest", "[txt]")
{
	string myTxt =
	"\n"
	" // comment "
	"\n"
	"[MySection] // comment  \n"
	"Hello = 23 // comment\n"
	"WESTRING_RACE_SELECTABLE=Auswählbar" // check Umlaute
	;

	map::Txt txt;
	std::basic_stringstream<byte> sstream;
	sstream << myTxt;
	//std::cout << sstream.str() << std::endl;
	txt.read(sstream);

	REQUIRE(txt.sections().size() == 1);
	const map::Txt::Section &section = txt.sections()[0];
	REQUIRE(section.name == "MySection");
	REQUIRE(section.entries.size() == 2);
	REQUIRE(txt.sections()[0].entries[0].key() == "Hello");
	REQUIRE(txt.sections()[0].entries[0].value() == "23");
}

TEST_CASE("TxtReadTest", "[txt]")
{
	string myTxt =
	"[MySection]\n"
	"Hello = 23\n"
	"\n"
	"\n"
	"// does our ruin benefit the world?\n"
	"\n"
	"Haha = 12// is there an avenging power in nature?\n"
	"\n"
	;

	map::Txt txt;
	std::basic_stringstream<byte> sstream;
	sstream << myTxt;
	//std::cout << sstream.str() << std::endl;
	txt.read(sstream);

	REQUIRE(txt.sections().size() == 1);
	const map::Txt::Section &section = txt.sections()[0];
	REQUIRE(section.name == "MySection");
	REQUIRE(section.entries.size() == 2);
	wc3lib::map::Txt::Entries::const_iterator begin = section.entries.begin();
	wc3lib::map::Txt::Entries::const_iterator end = section.entries.end();
	REQUIRE(std::find_if(begin, end, isHello) != end);
	//std::cout << "Value: " << std::find_if(begin, end, isHello)->second << std::endl;
	REQUIRE(std::find_if(begin, end, isHello)->second == "23");

	REQUIRE(std::find_if(begin, end, isHaha) != end);
	//std::cout << "Value: " << std::find_if(begin, end, isHaha)->second << std::endl;
	REQUIRE(std::find_if(begin, end, isHaha)->second == "12");

}

TEST_CASE("TxtReadTestWithSpacesAtBeginning", "[txt]")
{
	string myTxt =
	"\n\n\n\n "
	"[MySection]// Hey, what's this?\n"
	"Hello = 23\n"
	"\n"
	"\n"
	"// does our ruin benefit the world?\n"
	"\n"
	"Haha = 12// is there an avenging power in nature?\n"
	"\n"
	;

	map::Txt txt;
	std::basic_stringstream<byte> sstream;
	sstream << myTxt;
	//std::cout << sstream.str() << std::endl;
	txt.read(sstream);

	REQUIRE(txt.sections().size() == 1);
	const map::Txt::Section &section = txt.sections()[0];
	REQUIRE(section.name == "MySection");
	REQUIRE(section.entries.size() == 2);
	wc3lib::map::Txt::Entries::const_iterator begin = section.entries.begin();
	wc3lib::map::Txt::Entries::const_iterator end = section.entries.end();
	REQUIRE(std::find_if(begin, end, isHello) != end);
	//std::cout << "Value: " << std::find_if(begin, end, isHello)->second << std::endl;
	REQUIRE(std::find_if(begin, end, isHello)->second == "23");

	REQUIRE(std::find_if(begin, end, isHaha) != end);
	//std::cout << "Value: " << std::find_if(begin, end, isHaha)->second << std::endl;
	REQUIRE(std::find_if(begin, end, isHaha)->second == "12");

}

bool isBla(const map::Txt::Entry &entry)
{
	return entry.first == "bla";
}

bool isOperatorCompareBoolean(const map::Txt::Entry &entry)
{
	return entry.first == "OperatorCompareBoolean";
}

bool isOperatorCompareDestructibleHint(const map::Txt::Entry &entry)
{
	return entry.first == "OperatorCompareDestructibleHint";
}

TEST_CASE("TxtReadTriggerDataTest", "[txt]")
{
	string myTxt =
	"//***************************************************************************\n"
	"[bla]\n"

	"OperatorCompareBoolean=\"Boolean Comparison\"\n"
	"OperatorCompareBoolean=~Value,\" \",~Operator,\" \",~Value\n"

	"OperatorCompareBooleanHint = 10\n"
	"\n"
	"OperatorCompareDestructible=\"Destructible Comparison\"\n"
	"OperatorCompareDestructible=~Value,\" \",~Operator,\" \",~Value\n"
	"OperatorCompareDestructibleHint=\n"
	;

	map::Txt txt;
	std::basic_stringstream<byte> sstream;
	sstream << myTxt;
	//std::cout << sstream.str() << std::endl;
	txt.read(sstream);

	REQUIRE(txt.sections().size() == 1);
	const map::Txt::Section &section = txt.sections()[0];
	REQUIRE(section.name == "bla");
	//std::cout << "Entries count: " << txt.entries("bla").size()  << std::endl;
	REQUIRE(section.entries.size() == 6);
	wc3lib::map::Txt::Entries::const_iterator begin = section.entries.begin();
	wc3lib::map::Txt::Entries::const_iterator end = section.entries.end();
	REQUIRE(std::find_if(begin, end, isOperatorCompareBoolean) != end);
	//std::cout << "Value: " << std::find_if(begin, end, isOperatorCompareBoolean)->second << std::endl;
	REQUIRE(std::find_if(begin, end, isOperatorCompareBoolean)->second == "\"Boolean Comparison\"");

	REQUIRE(std::find_if(begin, end, isOperatorCompareDestructibleHint) != end);
	//std::cout << "Value: " << std::find_if(begin, end, isOperatorCompareDestructibleHint)->second << std::endl;
	REQUIRE(std::find_if(begin, end, isOperatorCompareDestructibleHint)->second == "");
}

TEST_CASE("TxtWriteTriggerDataTest", "[.][txt]")
{
	string myTxt =
	"//***************************************************************************\n"
	"[bla]\n"

	"OperatorCompareBoolean=\"Boolean Comparison\"\n"
	"OperatorCompareBoolean=~Value,\" \",~Operator,\" \",~Value\n"

	"OperatorCompareBooleanHint = 10\n"
	"\n"
	"OperatorCompareDestructible=\"Destructible Comparison\"\n"
	"OperatorCompareDestructible=~Value,\" \",~Operator,\" \",~Value\n"
	"OperatorCompareDestructibleHint=\n"
	;

	map::Txt txt;
	std::basic_stringstream<byte> sstream;
	sstream << myTxt;
	//std::cout << sstream.str() << std::endl;
	txt.read(sstream);

	REQUIRE(txt.sections().size() == 1);

	//std::cout << "test 1" << std::endl;

	sstream = std::basic_stringstream<byte>(std::stringstream::out | std::stringstream::in | std::stringstream::binary);
	txt.write(sstream);

	//ofstream out("bla.txt", std::fstream::out | std::fstream::binary);
	//txt.write(out);
	//out.close();

	//std::cout << "test 2" << std::endl;

	txt.sections().clear(); // clear Txt for new read

	std::cout << "newly written:\n" << sstream.str() << std::endl;

	REQUIRE(txt.sections().size() == 0);

	txt.read(sstream); // read again

	/*
	std::cout << "Sections:" << std::endl;

	FOREACH (map::Txt::Sections::const_reference ref, txt.sections()) {
		std::cout << ref.name << std::endl;
	}
	*/

	REQUIRE(txt.sections().size() == 1);
	const map::Txt::Section &section = txt.sections()[0];
	REQUIRE(section.name == "bla");
	//std::cout << "Entries count: " << txt.entries("bla").size()  << std::endl;
	const map::Txt::Sections::const_iterator iterator = std::find_if(txt.sections().begin(), txt.sections().end(), [](const map::Txt::Section &section) { return section.name == "bla"; });
	REQUIRE(iterator != txt.sections().end());
	REQUIRE(iterator->entries.size() == 6);
	const wc3lib::map::Txt::Entries::const_iterator begin = iterator->entries.begin();
	const wc3lib::map::Txt::Entries::const_iterator end = iterator->entries.end();
	REQUIRE(std::find_if(begin, end, isOperatorCompareBoolean) != end);
	//std::cout << "Value: " << std::find_if(begin, end, isOperatorCompareBoolean)->second << std::endl;
	REQUIRE(std::find_if(begin, end, isOperatorCompareBoolean)->second == "\"Boolean Comparison\"");

	REQUIRE(std::find_if(begin, end, isOperatorCompareDestructibleHint) != end);
	//std::cout << "Value: " << std::find_if(begin, end, isOperatorCompareDestructibleHint)->second << std::endl;
	REQUIRE(std::find_if(begin, end, isOperatorCompareDestructibleHint)->second == "");
}

TEST_CASE("EmptySection", "[txt]")
{
	string myTxt =
	"[nnom]\n"
	"\n"
	"// Goblin Laboratory\n"
	"[ngad]\n"
	"Art=ReplaceableTextures\\CommandButtons\\BTNAmmoDump.blp\n"
	"Sellunits=ngsp,nzep,ngir\n"
	;

	map::Txt txt;
	std::basic_stringstream<byte> sstream;
	sstream << myTxt;
	bool valid = true;

	try
	{
		txt.read(sstream);
	}
	catch (const Exception &e)
	{
		valid = false;
		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
}

/*
 * We test the German "WorldEditStrings.txt" file which needs UTF-8 support and contains many many entries.
 * This is the most realistic test since it uses an original file from The Frozen Throne.
 */
TEST_CASE("WorldEditStrings", "[txt]")
{

	ifstream in("WorldEditStrings.txt");

	REQUIRE(in);

	map::Txt txt;
	bool valid = true;
	const std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

	try
	{
		txt.read(in);
	}
	catch (Exception &e)
	{
		std::cerr << e.what() << std::endl;
		valid = false;
	}

	REQUIRE(valid);

	const std::chrono::high_resolution_clock::time_point finished = std::chrono::high_resolution_clock::now();
	const std::chrono::high_resolution_clock::duration duration = finished - now;

	std::cout << "WorldEditStrings.txt Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms" << std::endl;
	std::cout << "WorldEditStrings.txt Duration: " << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " s" << std::endl;
}

TEST_CASE("WorldEditData", "[.][txt]")
{

	ifstream in("WorldEditData.txt");

	REQUIRE(in);

	map::Txt txt;
	bool valid = true;
	const std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

	try
	{
		txt.read(in);
	}
	catch (Exception &e)
	{
		std::cerr << e.what() << std::endl;
		valid = false;
	}

	REQUIRE(valid);
	REQUIRE(txt.sections().size() == 38);
	const map::Txt::Section &section = txt.sections()[0];
	REQUIRE(section.name == "MySection");
	REQUIRE(section.entries.size() == 2);
	REQUIRE(txt.sections()[0].entries[0].key() == "Hello");
	REQUIRE(txt.sections()[0].entries[0].value() == "23");

	const std::chrono::high_resolution_clock::time_point finished = std::chrono::high_resolution_clock::now();
	const std::chrono::high_resolution_clock::duration duration = finished - now;

	std::cout << "WorldEditData.txt Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms" << std::endl;
	std::cout << "WorldEditData.txt Duration: " << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << " s" << std::endl;
}

TEST_CASE("UnitEditorData", "[txt]")
{
	ifstream in("UnitEditorData.txt");

	REQUIRE(in);

	map::Txt txt;
	bool valid = true;

	try
	{
		txt.read(in);
	}
	catch (Exception &e)
	{
		std::cerr << e.what() << std::endl;
		valid = false;
	}

	REQUIRE(valid);
}

TEST_CASE("NeutralUnitFunc", "[txt]")
{
	ifstream in("NeutralUnitFunc.txt");

	REQUIRE(in);

	map::Txt txt;
	bool valid = true;

	try
	{
		txt.read(in);
	}
	catch (Exception &e)
	{
		std::cerr << e.what() << std::endl;
		valid = false;
	}

	REQUIRE(valid);
}

// has a comment starting with one single /: / Stuffed Penguin
TEST_CASE("ItemFunc", "[txt]")
{
	ifstream in("ItemFunc.txt");

	REQUIRE(in);

	map::Txt txt;
	bool valid = true;

	try
	{
		txt.read(in);
	}
	catch (Exception &e)
	{
		std::cerr << e.what() << std::endl;
		valid = false;
	}

	REQUIRE(valid);
}

/*
[mdpb]
[mdpb]
Name=Medusa Pebble
Purchase Medusa Pebble
Hotkey=P
Tip=Purchase Medusa |cffffcc00P|rebble
Ubertip="Turns the targeted enemy non-Hero unit into stone when used. A unit turned to stone by the Medusa Pebble is removed from the game permanently."
Description=Turns target unit to stone.
*/
TEST_CASE("ItemStrings", "[txt]")
{
	ifstream in("ItemStrings.txt");

	REQUIRE(in);

	map::Txt txt;
	bool valid = true;

	try
	{
		txt.read(in);
	}
	catch (Exception &e)
	{
		std::cerr << e.what() << std::endl;
		valid = false;
	}

	REQUIRE(valid);
}


