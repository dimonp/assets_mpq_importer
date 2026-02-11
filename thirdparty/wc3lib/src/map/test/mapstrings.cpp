/***************************************************************************
 *   Copyright (C) 2014 by Tamino Dauth                                    *
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

#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "../../spirit.hpp"
#include "../../platform.hpp"
#include "../mapstrings.hpp"

using namespace wc3lib;

TEST_CASE("ok", "[mapstrings]")
{
	REQUIRE(true);
}

/*
 * German strings of War Chasers, a Warcraft III: Reign of Chaos scenario map by Blizzard Entertainment.
 */
TEST_CASE("WarChasersRead", "[.][mapstrings]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("mapstrings_warchasers_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("war3map.wts");

	REQUIRE(in);

	map::MapStrings strings;

	bool valid = true;

	try
	{
		strings.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(!strings.entries().empty());
	CHECK(strings.entries()[0].key == 2);
	CHECK(strings.entries()[0].comment == "");
	//std::cerr << "Value: " << strings.entries()[0].value << std::endl;
	CHECK(strings.entries()[0].value == "Spieler 1");

	CHECK(strings.entries().back().key == 178);
	CHECK(strings.entries().back().comment == "");
	//std::cerr << "Value: " << strings.entries()[0].value << std::endl;
	CHECK(strings.entries().back().value == "Dungeon-Bewohner");
}

TEST_CASE("WarChasersReadWriteRead", "[.][mapstrings]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("mapstrings_warchasers_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("war3map.wts");

	REQUIRE(in);

	map::MapStrings strings;

	bool valid = true;

	try
	{
		strings.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(!strings.entries().empty());
	CHECK(strings.entries()[0].key == 2);
	CHECK(strings.entries()[0].comment == "");
	//std::cerr << "Value: " << strings.entries()[0].value << std::endl;
	CHECK(strings.entries()[0].value == "Spieler 1");

	CHECK(strings.entries().back().key == 178);
	CHECK(strings.entries().back().comment == "");
	//std::cerr << "Value: " << strings.entries()[0].value << std::endl;
	CHECK(strings.entries().back().value == "Dungeon-Bewohner");

	ofstream out("war3mapout.wts");
	REQUIRE(out);

	try
	{
		strings.write(out);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	out.close();

	in.close();
	in.open("war3mapout.wts");

	REQUIRE(in);

	strings.clear();

	try
	{
		strings.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(!strings.entries().empty());
	CHECK(strings.entries()[0].key == 2);
	CHECK(strings.entries()[0].comment == "");
	//std::cerr << "Value: " << strings.entries()[0].value << std::endl;
	CHECK(strings.entries()[0].value == "Spieler 1");

	CHECK(strings.entries().back().key == 178);
	CHECK(strings.entries().back().comment == "");
	//std::cerr << "Value: " << strings.entries()[0].value << std::endl;
	CHECK(strings.entries().back().value == "Dungeon-Bewohner");
}

/*
 * German strings of Skibbi's Castle TD, a The Frozen Throne map.
 * This string file includes comments.
 */
TEST_CASE("SkibbisCastleTD", "[.][mapstrings]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("mapstrings_skibbiscastletd_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("SkibisCastleTD/war3map.wts");

	REQUIRE(in);

	map::MapStrings strings;

	bool valid = true;

	try
	{
		strings.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(!strings.entries().empty());
	REQUIRE(strings.entries()[0].key == 0);
	//std::cerr << "Comment: " << strings.entries()[0].comment << std::endl;
	// initial spaces of comments are skipped
	REQUIRE(strings.entries()[0].comment == "Units: h02K (D-String), Name (Name)");
	//std::cerr << "Value: " << strings.entries()[0].value << std::endl;
	REQUIRE(strings.entries()[0].value == "D-Kette");
}

// Talras has a very big strings file.
TEST_CASE("TalrasReadGerman", "[.][mapstrings]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("mapstrings_talrasde_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("war3map_Talras_de.wts");

	REQUIRE(in);

	map::MapStrings strings;

	REQUIRE_NOTHROW(strings.read(in));
	REQUIRE(!strings.entries().empty());
}

TEST_CASE("TalrasReadEnglish", "[.][mapstrings]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("mapstrings_talrasen_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("war3map_Talras_en.wts");

	REQUIRE(in);

	map::MapStrings strings;

	REQUIRE_NOTHROW(strings.read(in));
	REQUIRE(!strings.entries().empty());
}
