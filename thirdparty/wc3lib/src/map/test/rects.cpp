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

#include <fstream>
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../../platform.hpp"
#include "../rects.hpp"

using namespace wc3lib;

TEST_CASE("ReadTest")
{
	ifstream in("war3map.w3r", ifstream::in | ifstream::binary); // TestMap.w3m

	REQUIRE(in);

	map::Rects rects;

	bool valid = true;

	try
	{
		rects.read(in);
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);

	REQUIRE(rects.rects().size() == 3);
	const map::Rect &rect = rects.rects().at(0);
	REQUIRE(rect.name() == "Blue Rect");
	REQUIRE(rect.color() == 0xFF0000); // blue - BGR color!
	REQUIRE(!rect.hasWeatherEffect());
	REQUIRE(rect.soundName().empty());
	CHECK_THAT(rect.left(), Catch::Matchers::WithinRel( 1120.0, 0.0001));
	CHECK_THAT(rect.right(), Catch::Matchers::WithinRel(1152.0, 0.0001));
	CHECK_THAT(rect.bottom(), Catch::Matchers::WithinRel(896.0, 0.0001));
	CHECK_THAT(rect.top(), Catch::Matchers::WithinRel(928.0, 0.0001));

	const map::Rect &whiteRect = rects.rects().at(1);
	REQUIRE(whiteRect.name() == "White Rect");
	REQUIRE(whiteRect.color() == 0xFFFFFF); // white - BGR color!
	REQUIRE(!whiteRect.hasWeatherEffect());
	REQUIRE(whiteRect.soundName().empty());
	CHECK_THAT(whiteRect.left(), Catch::Matchers::WithinRel(1024.0, 0.0001));
	CHECK_THAT(whiteRect.right(), Catch::Matchers::WithinRel(1152.0, 0.0001));
	CHECK_THAT(whiteRect.bottom(), Catch::Matchers::WithinRel(768.0, 0.0001));
	CHECK_THAT(whiteRect.top(), Catch::Matchers::WithinRel(896.0, 0.0001));

	const map::Rect &redRectWithWeathereffect = rects.rects().at(2);
	REQUIRE(redRectWithWeathereffect.name() == "Red Rect With Weathereffect");
	REQUIRE(redRectWithWeathereffect.color() == 0x0000FF); // red - BGR color!
	REQUIRE(redRectWithWeathereffect.hasWeatherEffect());
	REQUIRE(map::idToString(redRectWithWeathereffect.weatherEffectId()) == "RAhr"); // taken from "TERRAINART/Weather.slk" Ashenvale Heavy Rain
	REQUIRE(redRectWithWeathereffect.hasSound());
	REQUIRE(redRectWithWeathereffect.soundName() == "gg_snd_RainOfFireLoop1");
	CHECK_THAT(redRectWithWeathereffect.left(), Catch::Matchers::WithinRel(896.0, 0.0001));
	CHECK_THAT(redRectWithWeathereffect.right(), Catch::Matchers::WithinRel(1024.0, 0.0001));
	CHECK_THAT(redRectWithWeathereffect.bottom(), Catch::Matchers::WithinRel(768.0, 0.0001));
	CHECK_THAT(redRectWithWeathereffect.top(), Catch::Matchers::WithinRel(992.0, 0.0001));
}

TEST_CASE("ReadWriteReadTest")
{
	ifstream in("war3map.w3r", ifstream::in | ifstream::binary); // Reign of Chaos

	REQUIRE(in);

	map::Rects rects;

	bool valid = true;

	try
	{
		rects.read(in);
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);

	in.close();
	ofstream out("war3map.w3rout", ifstream::out | ifstream::binary);

	REQUIRE(out);

	try
	{
		rects.write(out);
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);

	out.close(); // flush file stream
	rects.rects().clear(); // ensure it's empty!

	in.open("war3map.w3rout", ifstream::in | ifstream::binary); // Reign of Chaos, reopen

	REQUIRE(in);

	try
	{
		rects.read(in);

	}
	catch (...) {
		valid = false;
	}

	REQUIRE(valid);

	REQUIRE(rects.rects().size() == 3);
	const map::Rect &rect = rects.rects().at(0);
	REQUIRE(rect.name() == "Blue Rect");
	REQUIRE(rect.color() == 0xFF0000); // blue - BGR color!
	REQUIRE(!rect.hasWeatherEffect());
	REQUIRE(rect.soundName().empty());
	CHECK_THAT(rect.left(), Catch::Matchers::WithinRel(1120.0, 0.0001));
	CHECK_THAT(rect.right(), Catch::Matchers::WithinRel(1152.0, 0.0001));
	CHECK_THAT(rect.bottom(), Catch::Matchers::WithinRel(896.0, 0.0001));
	CHECK_THAT(rect.top(), Catch::Matchers::WithinRel(928.0, 0.0001));

	const map::Rect &whiteRect = rects.rects().at(1);
	REQUIRE(whiteRect.name() == "White Rect");
	REQUIRE(whiteRect.color() == 0xFFFFFF); // white - BGR color!
	REQUIRE(!whiteRect.hasWeatherEffect());
	REQUIRE(whiteRect.soundName().empty());
	CHECK_THAT(whiteRect.left(), Catch::Matchers::WithinRel(1024.0, 0.0001));
	CHECK_THAT(whiteRect.right(), Catch::Matchers::WithinRel(1152.0, 0.0001));
	CHECK_THAT(whiteRect.bottom(), Catch::Matchers::WithinRel(768.0, 0.0001));
	CHECK_THAT(whiteRect.top(), Catch::Matchers::WithinRel(896.0, 0.0001));

	const map::Rect &redRectWithWeathereffect = rects.rects().at(2);
	REQUIRE(redRectWithWeathereffect.name() == "Red Rect With Weathereffect");
	REQUIRE(redRectWithWeathereffect.color() == 0x0000FF); // red - BGR color!
	REQUIRE(redRectWithWeathereffect.hasWeatherEffect());
	REQUIRE(map::idToString(redRectWithWeathereffect.weatherEffectId()) == "RAhr"); // taken from "TERRAINART/Weather.slk" Ashenvale Heavy Rain
	REQUIRE(redRectWithWeathereffect.hasSound());
	REQUIRE(redRectWithWeathereffect.soundName() == "gg_snd_RainOfFireLoop1");
	CHECK_THAT(redRectWithWeathereffect.left(), Catch::Matchers::WithinRel(896.0, 0.0001));
	CHECK_THAT(redRectWithWeathereffect.right(), Catch::Matchers::WithinRel(1024.0, 0.0001));
	CHECK_THAT(redRectWithWeathereffect.bottom(), Catch::Matchers::WithinRel(768.0, 0.0001));
	CHECK_THAT(redRectWithWeathereffect.top(), Catch::Matchers::WithinRel(992.0, 0.0001));
}
