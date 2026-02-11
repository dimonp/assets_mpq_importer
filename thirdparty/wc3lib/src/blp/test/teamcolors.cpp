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

/**
 * \file
 * Unit Test which reads all team color files.
 * Those files have been analyzed by external tools:
 * - size is 8x8
 * - 4 MIP maps
 * - BLP1
 * - JPEG compression
 * - JPEG header size: 624 bytes
 */
#include <iostream>
#include <fstream>
#include <catch2/catch_test_macros.hpp>

#include "../blp.hpp"

using namespace wc3lib;

/*
 * Red
 */
TEST_CASE("TeamColor00")
{
	std::ifstream in("TeamColor00.blp", std::ifstream::in | std::ifstream::binary);

	REQUIRE(in);

	blp::Blp texture;
	bool valid = true;

	try
	{
		texture.read(in);
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	REQUIRE(texture.format() == blp::Blp::Format::Blp1);
	REQUIRE(texture.compression() == blp::Blp::Compression::Jpeg);
	REQUIRE(texture.mipMaps().size() == 4);
	REQUIRE(texture.mipMaps()[0].width() == 8);
	REQUIRE(texture.mipMaps()[0].height() == 8);
	REQUIRE(texture.mipMaps()[0].colorAt(0, 0).rgba() == 0xFF0303FF); // red
}

TEST_CASE("TeamColor00Write")
{
	std::ifstream in("TeamColor00.blp", std::ifstream::in | std::ifstream::binary);

	REQUIRE(in);

	blp::Blp texture;
	bool valid = true;

	try
	{
		texture.read(in);
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	in.close();

	REQUIRE(valid);

	std::ofstream out("TeamColor00tmp.blp", std::ifstream::binary);

	REQUIRE(out);

	try
	{
		texture.write(out);
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	out.close();

	REQUIRE(valid);

	// now read again
	in.open("TeamColor00tmp.blp", std::ifstream::in | std::ifstream::binary);

	REQUIRE(in);

	try
	{
		texture.read(in);
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);

	REQUIRE(texture.format() == blp::Blp::Format::Blp1);
	REQUIRE(texture.compression() == blp::Blp::Compression::Jpeg);
	REQUIRE(texture.mipMaps().size() == 4);
	REQUIRE(texture.mipMaps()[0].width() == 8);
	REQUIRE(texture.mipMaps()[0].height() == 8);
	std::cerr << std::hex << "Color as hex: " << texture.mipMaps()[0].colorAt(0, 0).rgba() << std::endl;
	std::cerr.unsetf(std::ios::hex);
	REQUIRE(texture.mipMaps()[0].colorAt(0, 0).rgba() == 0xFF0303FF); // red
}

TEST_CASE("TeamColor00WriteWithoutSharedHeader")
{
	std::ifstream in("TeamColor00.blp", std::ifstream::in | std::ifstream::binary);

	REQUIRE(in);

	blp::Blp texture;
	bool valid = true;

	try
	{
		texture.read(in);
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	in.close();

	REQUIRE(valid);

	std::ofstream out("TeamColor00tmp.blp", std::ifstream::binary);

	REQUIRE(out);

	try
	{
		texture.write(out, blp::Blp::defaultQuality, blp::Blp::defaultMipMaps, false);
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	out.close();

	REQUIRE(valid);

	// now read again
	in.open("TeamColor00tmp.blp", std::ifstream::in | std::ifstream::binary);

	REQUIRE(in);

	try
	{
		texture.read(in);
	}
	catch (std::exception &e)
	{
		valid = false;

		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);

	REQUIRE(texture.format() == blp::Blp::Format::Blp1);
	REQUIRE(texture.compression() == blp::Blp::Compression::Jpeg);
	REQUIRE(texture.mipMaps().size() == 4);
	REQUIRE(texture.mipMaps()[0].width() == 8);
	REQUIRE(texture.mipMaps()[0].height() == 8);
	std::cerr << std::hex << "Color as hex: " << texture.mipMaps()[0].colorAt(0, 0).rgba() << std::endl;
	std::cerr.unsetf(std::ios::hex);
	REQUIRE(texture.mipMaps()[0].colorAt(0, 0).rgba() == 0xFF0303FF); // red
	REQUIRE(blp::red(texture.mipMaps()[0].colorAt(0, 0).rgba()) == 0xFF);
	REQUIRE(blp::green(texture.mipMaps()[0].colorAt(0, 0).rgba()) == 0x03);
	REQUIRE(blp::blue(texture.mipMaps()[0].colorAt(0, 0).rgba()) == 0x03);
	REQUIRE(blp::alpha(texture.mipMaps()[0].colorAt(0, 0).rgba()) == 0xFF);
}
