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
#include <chrono>

#include "../../spirit.hpp"
#include "../../platform.hpp"
#include "../../exception.hpp"
#include "../slk.hpp"

using namespace wc3lib;

TEST_CASE("ConvertStrings", "[.][slk]")
{
    CHECK(map::Slk::fromSlkString("\"ABC\"") == "ABCS");
    CHECK(map::Slk::fromSlkString("\"\"") == "");
    CHECK(map::Slk::fromSlkString("\"") == "");
    CHECK(map::Slk::fromSlkString("") == "");
    CHECK(map::Slk::toSlkString("") == "\"\"");
    CHECK(map::Slk::toSlkString("ABC") == "\"ABC\"");
    CHECK(map::Slk::toSlkString("\"ABC\"") == "\"ABC\"");
}

TEST_CASE("UnitAbilities", "[slk]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("unitabilities_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("UnitAbilities.slk");

	REQUIRE(in);

	map::Slk slk;

	bool valid = true;

	try
	{
		slk.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(slk.columns() == 7);
	REQUIRE(slk.rows() == 325);
	REQUIRE(slk.cell(0, 0) == "\"unitAbilID\"");
}

TEST_CASE("UnitBalance", "[slk]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("unitbalance_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("UnitBalance.slk");

	REQUIRE(in);

	map::Slk slk;

	bool valid = true;

	try
	{
		slk.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(slk.columns() == 44);
	REQUIRE(slk.rows() == 467);
	REQUIRE(slk.cell(0, 0) == "\"unitBalanceID\"");
}

TEST_CASE("UnitData", "[slk]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("unitdata_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("UnitData.slk");

	REQUIRE(in);

	map::Slk slk;

	bool valid = true;

	try
	{
		slk.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(slk.columns() == 34);
	REQUIRE(slk.rows() == 467);
	REQUIRE(slk.cell(0, 0) == "\"unitID\"");

	/*
	 * Moon Priestess entry uses F records to set the position before
	 * specifying the values with K records.
	 */
	REQUIRE(slk.cell(66, 2) == "\"HeroMoonPriestess\"");
}

TEST_CASE("UnitMetaData", "[slk]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("unitmetadata_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("UnitMetaData.slk");

	REQUIRE(in);

	map::Slk slk;

	bool valid = true;

	try
	{
		slk.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	// 17 columns
	// 118 rows
	REQUIRE(valid);
	REQUIRE(slk.columns() == 17);
	REQUIRE(slk.rows() == 118);
	REQUIRE(slk.cell(0, 0) == "\"ID\"");
	REQUIRE(slk.cell(0, 1) == "\"field\"");
}

TEST_CASE("UnitMetaDataLibreOffice", "[slk]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("unitmetadatalibreoffice_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("UnitMetaDataExportedByLibreOffice.slk");

	REQUIRE(in);

	map::Slk slk;

	bool valid = true;

	try
	{
		slk.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	// 17 columns
	// 118 rows
	REQUIRE(valid);
	REQUIRE(slk.columns() == 17);
	REQUIRE(slk.rows() == 118);
	REQUIRE(slk.cell(0, 0) == "\"ID\"");
	REQUIRE(slk.cell(0, 1) == "\"field\"");
}

TEST_CASE("UnitWeapons", "[slk]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("unitweapons_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("UnitWeapons.slk");

	REQUIRE(in);

	map::Slk slk;

	bool valid = true;

	try
	{
		slk.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(slk.columns() == 67);
	REQUIRE(slk.rows() == 317);
	REQUIRE(slk.cell(0, 0) == "\"unitWeapID\"");
	REQUIRE(slk.cell(0, 1) == "\"sortWeap\"");
}

/*
 * Frozen Throne
 */
TEST_CASE("UnitMetaDataEX", "[slk]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("unitmetadataex_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("UnitMetaDataEx.slk");

	REQUIRE(in);

	map::Slk slk;

	bool valid = true;

	try
	{
		slk.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(slk.columns() == 23);
	REQUIRE(slk.rows() == 249);
	REQUIRE(slk.cell(0, 0) == "\"ID\"");
	REQUIRE(slk.cell(0, 1) == "\"field\"");
}

/*
 * Frozen Throne:
 * Time value after starting multiple grammars the grammar should always be initialized statically.
 */
TEST_CASE("UnitMetaDataEXProfile", "[slk]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("unitmetadataex_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("UnitMetaDataEx.slk");

	REQUIRE(in);

	map::Slk slk;

	bool valid = true;

	try
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		slk.read(in);
		std::chrono::high_resolution_clock::time_point finished = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::duration duration = finished - now;
		std::cerr << "Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms" << std::endl;
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(slk.columns() == 23);
	REQUIRE(slk.rows() == 249);
	REQUIRE(slk.cell(0, 0) == "\"ID\"");
	REQUIRE(slk.cell(0, 1) == "\"field\"");
}
