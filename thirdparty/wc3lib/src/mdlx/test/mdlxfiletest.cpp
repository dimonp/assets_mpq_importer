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
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <iostream>

#include "../mdlx.hpp"

using namespace wc3lib;
using namespace wc3lib::mdlx;

namespace
{

void verifyOrcExp(const Mdlx &model)
{
	/*
	 * Sequences 2 {
	 * GlobalSequences 17 {
	 * Textures 23 {
	 * Materials 26 {
	 * TextureAnims 1 {
	 * PivotPoints 168 {
	 * 4 lights
	 * 12 helpers
	 * 1 camera
	 * 3 event objects
	 * 6 geoset animations
	 * ?? geosets
	 * ?? bones
	 */
	CHECK_THAT(model.model().name(), Catch::Matchers::Equals("Orc_Exp"));
	CHECK(model.modelVersion() == 800);
	CHECK(model.sequences().size() == 2);
	CHECK_THAT(model.sequences()[0].name(), Catch::Matchers::Equals("Stand"));
	CHECK_THAT(model.sequences()[1].name(), Catch::Matchers::Equals("Birth"));
	CHECK(model.globalSequences().size() == 17);
	CHECK(model.textures().size() == 23);
	CHECK(model.materials().size() == 26);
}

}

TEST_CASE("Orc_Exp_Mdx", "[mdlxfiletest]")
{
	std::cout << "Orc_Exp_Mdx" << std::endl;
	std::ifstream in("Orc_Exp.mdx", std::ifstream::in | std::ifstream::binary);

	REQUIRE(in);

	Mdlx model;
	bool valid = true;
	std::size_t size = 0;

	try
	{
		size = static_cast<std::size_t>(model.read(in));
	}
	catch (const Exception &e)
	{
		valid = false;
		std::cerr << e.what() << std::endl;
	}

	in.close();

	REQUIRE(valid);

	verifyOrcExp(model);

	INFO("Writing file Orc_Exp_out.mdx");

	std::ofstream out("Orc_Exp_out.mdx", std::ifstream::binary);

	REQUIRE(out);

	valid = true;
	size = 0;

	try
	{
		size = static_cast<std::size_t>(model.write(out));
	}
	catch (const Exception &e)
	{
		valid = false;
		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);

	INFO("Reading file Orc_Exp_out.mdx");

	std::cerr << "Read written file." << std::endl;

	// model = Mdlx();
	// valid = true;
	// size = 0;
	// in.open("Orc_Exp_out.mdx", std::ifstream::in | std::ifstream::binary);

	// REQUIRE(in);

	// try
	// {
	// 	size = static_cast<std::size_t>(model.read(in));
	// }
	// catch (const Exception &e)
	// {
	// 	valid = false;
	// 	std::cerr << e.what() << std::endl;
	// }

	// REQUIRE(valid);
	// REQUIRE(size > 0);

	// verifyOrcExp(model);
}
