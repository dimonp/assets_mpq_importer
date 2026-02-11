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

#include <iostream>
#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../../platform.hpp"
#include "../info.hpp"

using namespace wc3lib;

TEST_CASE("InfoReadTest", "[info]")
{
	std::ifstream in("war3map.w3i");

	REQUIRE(in);

	map::Info info;
	bool valid = true;

	try
	{
		info.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(info.players().size() == 5);
	REQUIRE(info.forces().size() == 2);
	// TODO check the numbers
	//REQUIRE(info.upgradeAvailabilities().size() == 0);
	//REQUIRE(info.techAvailabilities().size() == 9); // 9 units are not available for night elves (enemies)
	//REQUIRE(info.techAvailabilities()[0].playerMask() == 11); // enemies TODO 11th bit must be set 1
	REQUIRE(info.randomUnitTables().size() == 0); // no custom tables
}

TEST_CASE("InfoReadWriteReadTest", "[info]")
{
	std::ifstream in("war3map.w3i");

	REQUIRE(in);

	map::Info info;
	bool valid = true;

	try
	{
		info.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(info.players().size() == 5);
	REQUIRE(info.forces().size() == 2);
	// TODO check the numbers
	//REQUIRE(info.upgradeAvailabilities().size() == 0);
	//REQUIRE(info.techAvailabilities().size() == 9); // 9 units are not available for night elves (enemies)
	//REQUIRE(info.techAvailabilities()[0].playerMask() == 11); // enemies TODO 11th bit must be set 1
	REQUIRE(info.randomUnitTables().size() == 0); // no custom tables

	const char *outFileName = "war3map.w3iout";
	std::ofstream out(outFileName);

	REQUIRE(out);

	valid = true;

	try
	{
		info.write(out);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);

	out.close();
	in.close();
	info.clear();
	in.open(outFileName);

	REQUIRE(in);

	valid = true;

	try
	{
		info.read(in);
	}
	catch (const Exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(info.players().size() == 5);
	REQUIRE(info.forces().size() == 2);
	// TODO check the numbers
	//REQUIRE(info.upgradeAvailabilities().size() == 0);
	//REQUIRE(info.techAvailabilities().size() == 9); // 9 units are not available for night elves (enemies)
	//REQUIRE(info.techAvailabilities()[0].playerMask() == 11); // enemies TODO 11th bit must be set 1
	REQUIRE(info.randomUnitTables().size() == 0); // no custom tables

	// TODO check all stuff again
}
