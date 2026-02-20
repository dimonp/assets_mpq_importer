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
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>

#include <boost/cast.hpp>
//#include <boost/foreach.hpp>

#include "../../platform.hpp"
#include "../w3m.hpp"
#include "../tilepoint.hpp"

using namespace wc3lib;

/**
 * \file
 * This test tries to open the War Chasers map, a standard Warcraft III: Reign of Chaos scenario and then tests all different file formats
 * in separate test cases.
 */
TEST_CASE("ReadTest")
{
	map::W3m map;

	bool valid = true;

	try
	{
		map.open("(4)WarChasers.w3m");
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);

	/*
	 * NOTE
	 * the mini map had decompression issues
	 */
	mpq::File file = map.findFile("war3mapMap.blp");

	REQUIRE(file.isValid());
	ofstream outInfo("minimap.info");

	REQUIRE(outInfo);

	outInfo << mpq::archiveInfo(map, true, true);
	outInfo << mpq::fileInfo(file, true, true);
	outInfo.close();
	ofstream outMinimap("war3mapMap.blp");

	REQUIRE(outMinimap);
	valid = true;

	try
	{
		file.decompress(outMinimap);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		valid = false;
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);
}

TEST_CASE("Cameras")
{
	map::W3m map;

	bool valid = true;

	try
	{
		map.open("(4)WarChasers.w3m");
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);

	/*
	 * Cameras
	 */
	bool validCameras = true;

	try
	{
		map.readFileFormat(map.cameras().get());
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		validCameras = false;
	}
	catch (...)
	{
		validCameras = false;
	}

	REQUIRE(validCameras);

	const auto& cameras = map.cameras()->cameras();

	REQUIRE(cameras.size() == 6);
	const auto &cam0 = cameras[0];
	REQUIRE(cam0->name() == "CamStart1");
	CHECK_THAT(cam0->targetX(), Catch::Matchers::WithinRel(-7718.41, 0.01));
	CHECK_THAT(cam0->targetY(), Catch::Matchers::WithinRel(-9039.14, 0.01));
	CHECK_THAT(cam0->zOffset(), Catch::Matchers::WithinRel(0.00, 0.01));
	CHECK_THAT(cam0->rotation(), Catch::Matchers::WithinRel(90.0, 0.01));
	CHECK_THAT(cam0->angleOfAttack(), Catch::Matchers::WithinRel(304.00, 0.01));
	CHECK_THAT(cam0->distance(), Catch::Matchers::WithinRel(1790.91, 0.01));
	CHECK_THAT(cam0->roll(), Catch::Matchers::WithinRel(0.00, 0.01));
	CHECK_THAT(cam0->fieldOfView(), Catch::Matchers::WithinRel(70.00, 0.01));
	CHECK_THAT(cam0->farZ(), Catch::Matchers::WithinRel(5000.00, 0.01));

	const auto &cam1 = cameras[1];
	REQUIRE(cam1->name() == "Camera 002");
	CHECK_THAT(cam1->targetX(), Catch::Matchers::WithinRel(2595.08, 0.01));
	CHECK_THAT(cam1->targetY(), Catch::Matchers::WithinRel(-8045.40, 0.01));
	CHECK_THAT(cam1->zOffset(), Catch::Matchers::WithinRel(0.00, 0.01));
	CHECK_THAT(cam1->rotation(), Catch::Matchers::WithinRel(133.47, 0.01));
	CHECK_THAT(cam1->angleOfAttack(), Catch::Matchers::WithinRel(314.55, 0.01));
	CHECK_THAT(cam1->distance(), Catch::Matchers::WithinRel(1996.50, 0.01));
	CHECK_THAT(cam1->roll(), Catch::Matchers::WithinRel(0.00, 0.01));
	CHECK_THAT(cam1->fieldOfView(), Catch::Matchers::WithinRel(70.00, 0.01));
	CHECK_THAT(cam1->farZ(), Catch::Matchers::WithinRel(5000.00, 0.01));
}

TEST_CASE("Rects")
{
	map::W3m map;

	bool valid = true;

	try
	{
		map.open("(4)WarChasers.w3m");
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);

	/*
	 * Rects
	 */
	bool validRects = true;

	try
	{
		map.readFileFormat(map.rects().get());
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		validRects = false;
	}
	catch (...)
	{
		validRects = false;
	}

	REQUIRE(validRects);
	// Object Manager lists this number
	REQUIRE(map.rects()->rects().size() == 151);
	const map::Rect &rect0 = map.rects()->rects()[0];
	REQUIRE(rect0.name() == "ALL SPIDERS");
	CHECK_THAT(rect0.left(), Catch::Matchers::WithinRel(-8000, 0.01));
	CHECK_THAT(rect0.right(), Catch::Matchers::WithinRel(-6400, 0.01));
	CHECK_THAT(rect0.bottom(), Catch::Matchers::WithinRel(2048, 0.01));
	CHECK_THAT(rect0.top(), Catch::Matchers::WithinRel(4512, 0.01));
	CHECK(rect0.color() == 0xFFFFFF);
	CHECK(!rect0.hasWeatherEffect());
	CHECK(!rect0.hasSound());
}

TEST_CASE("CustomUnits")
{
	map::W3m map;

	bool valid = true;

	try
	{
		map.open("(4)WarChasers.w3m");
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);

	/*
	 * Custom Units
	 */
	bool validCustomUnits = true;

	try
	{
		map.readFileFormat(map.customUnits().get());
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		validCustomUnits = false;
	}
	catch (...)
	{
		validCustomUnits = false;
	}

	REQUIRE(validCustomUnits);
	// Object Manager lists this number
	REQUIRE(map.customUnits()->originalTable().size() == 122);
	REQUIRE(map.customUnits()->customTable().size() == 25);

	/*
	 TODO add more precise tests
	const map::CustomUnits::Unit &unit0 = map.customUnits()->originalTable()[0];
	CHECK(map::idToString(unit0.originalId()) == "hmtt");
	CHECK(map::idToString(unit0.customId()) == "");
	*/
}

TEST_CASE("Environment")
{
	map::W3m map;

	bool valid = true;

	try
	{
		map.open("(4)WarChasers.w3m");
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		valid = false;
	}

	REQUIRE(valid);

	/*
	 * Environment
	 */
	bool validEnvironment = true;

	try
	{
		map.readFileFormat(map.environment().get());
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		validEnvironment = false;
	}
	catch (...)
	{
		validEnvironment = false;
	}

	REQUIRE(validEnvironment);
	// Map Info lists the numbers
	CHECK(strcmp(map.environment()->fileTextId(), "W3E!") == 0);
	CHECK(strcmp(map.environment()->fileName(), "war3map.w3e") == 0);
	CHECK(map.environment()->version() == 11);
	CHECK(map.environment()->mainTileset() == map::Environment::MainTileset::Cityscape);
	REQUIRE(map.environment()->customized());
	REQUIRE(map.environment()->cliffTilesetsIds().size() == 2);
	REQUIRE(map.environment()->groundTilesetsIds().size() == 11);
	// TODO check cliff tileset IDs and ground tileset IDs

	REQUIRE(map.environment()->mapHeight() == 160);
	REQUIRE(map.environment()->mapWidth() == 160);
	// TODO check further values
}
