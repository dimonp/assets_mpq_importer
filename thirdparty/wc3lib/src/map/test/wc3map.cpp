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

#include <boost/cast.hpp>

#include "../w3m.hpp"

using namespace wc3lib;

/**
 * \file
 * For this Unit Test a custom Warcraft III: The Reign of Chaos map has been created with test objects
 * which are queried in the test cases.
 */

TEST_CASE("Rects")
{
	map::W3m map;

	bool valid = true;

	try
	{
		map.open("TestMap.w3m");
		map.readFileFormat(map.rects().get());
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);
	REQUIRE(map.rects().get() != 0);
	REQUIRE(strcmp(map.rects()->fileName(), "war3map.w3r") == 0);
	std::cerr << "Rects:" << map.rects()->rects().size() << std::endl;
	REQUIRE(map.rects()->rects().size() == 3);
}
