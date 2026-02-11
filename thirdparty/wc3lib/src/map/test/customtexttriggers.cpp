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

#include <fstream>
#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "../../platform.hpp"
#include "../customtexttriggers.hpp"

using namespace wc3lib;

TEST_CASE("SimpleReadTest", "[customtexttriggers]") {
	ifstream in("war3map.wct", ifstream::in | ifstream::binary); // War Chasers

	REQUIRE(in);

	map::CustomTextTriggers customTextTriggers;

	bool valid = true;

	try {
		customTextTriggers.read(in);
	}
	catch (...) {
		valid = false;
	}

	REQUIRE(valid);
}

TEST_CASE("ReadWriteReadTest", "[customtexttriggers]") {
	ifstream in("war3map.wct", ifstream::in | ifstream::binary); // War Chasers

	REQUIRE(in);

	map::CustomTextTriggers customTextTriggers;

	bool valid = true;

	try {
		customTextTriggers.read(in);
	}
	catch (Exception &e) {
		valid = false;
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		valid = false;
	}

	REQUIRE(valid);

	in.close();
	ofstream out("war3map.wctout", ifstream::out | ifstream::binary);

	REQUIRE(out);

	try {
		customTextTriggers.write(out);
	}
	catch (Exception &e) {
		valid = false;
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		valid = false;
	}

	out.close();
	REQUIRE(valid);

	customTextTriggers.triggerTexts().clear(); // ensure it's empty!
	in.open("war3map.wctout", ifstream::in | ifstream::binary); // War Chasers, reopen

	REQUIRE(in);

	try {
		customTextTriggers.read(in);

	}
	catch (Exception &e) {
		valid = false;
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		valid = false;
	}

	REQUIRE(valid);
}
