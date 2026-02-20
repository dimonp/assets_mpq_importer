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

#include "../core.hpp"

using namespace wc3lib;

TEST_CASE("ReadInt")
{
	const int value = 10;
	stringstream sstream;
	sstream.write(reinterpret_cast<const byte*>(&value), sizeof(value));
	int result = 0;
	std::streamsize size = 0;
	wc3lib::read(sstream, result, size);
	CHECK(result == value);
	CHECK(size == sizeof(value));
}

/**
 * Tests the readString() function with 0 values.
 */
TEST_CASE("ReadString")
{
	// 4 times zero
	const string data = "0000";
	stringstream sstream;
	// write + 0 terminating character
	sstream.write(data.c_str(), static_cast<std::streamsize>(data.size() + 1));

	string result;
	std::streamsize size = 0;
	bool valid = true;

	try
	{
		readString(sstream, result, size, '\0', 1024);
	}
	catch (std::exception &e)
	{
		valid = false;
		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		valid = false;
		std::cerr << "Unknown exception!" << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(size == 5);
	REQUIRE(result.size() == 4);
	REQUIRE(result == "0000");

	const char characters[2] = { '0', 0 };
	result = "";
	size = 0;

	ofstream out("read.tmp", std::ios::out | std::ios::binary);

	REQUIRE(out);
	out.write(characters, 2);
	out.close();

	ifstream in("read.tmp", std::ios::binary | std::ios::in);

	REQUIRE(in);

	try
	{
		readString(in, result, size, '\0', 1024);
	}
	catch (std::exception &e)
	{
		valid = false;
		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		valid = false;
		std::cerr << "Unknown exception!" << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(size == 2);
	REQUIRE(result.size() == 1);
	REQUIRE(result == "0");
}
