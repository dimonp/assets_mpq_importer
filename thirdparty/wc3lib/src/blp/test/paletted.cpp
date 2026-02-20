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

#include "../blp.hpp"

using namespace wc3lib;

/*
 * BLP1 paletted + alpha
 */
TEST_CASE("HumanCampaignCastle", "[paletted]")
{
	std::ifstream in("HumanCampaignCastle.blp", std::ifstream::in | std::ifstream::binary);

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
	REQUIRE(texture.compression() == blp::Blp::Compression::Paletted);
	REQUIRE(texture.pictureType() == blp::Blp::PictureType::PalettedWithAlpha2);
	REQUIRE(texture.mipMaps().size() == 10);
	REQUIRE(texture.mipMaps()[0].width() == 512);
	REQUIRE(texture.mipMaps()[0].height() == 512);
}

TEST_CASE("HumanCampaignCastleWrite", "[paletted]")
{
	std::ifstream in("HumanCampaignCastle.blp", std::ifstream::in | std::ifstream::binary);

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
	REQUIRE(texture.format() == blp::Blp::Format::Blp1);
	REQUIRE(texture.compression() == blp::Blp::Compression::Paletted);
	REQUIRE(texture.pictureType() == blp::Blp::PictureType::PalettedWithAlpha2);
	REQUIRE(texture.mipMaps().size() == 10);
	REQUIRE(texture.mipMaps()[0].width() == 512);
	REQUIRE(texture.mipMaps()[0].height() == 512);

	std::ofstream out("HumanCampaignCastleOut.blp", std::ios::out | std::ios::binary);
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

	in.open("HumanCampaignCastleOut.blp", std::ios::in | std::ios::binary);

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

	in.close();
	REQUIRE(valid);
	REQUIRE(texture.format() == blp::Blp::Format::Blp1);
	REQUIRE(texture.compression() == blp::Blp::Compression::Paletted);
	REQUIRE(texture.pictureType() == blp::Blp::PictureType::PalettedWithAlpha2);
	REQUIRE(texture.mipMaps().size() == 10);
	REQUIRE(texture.mipMaps()[0].width() == 512);
	REQUIRE(texture.mipMaps()[0].height() == 512);
}
