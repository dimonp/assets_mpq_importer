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
 * Tests the basic serialization functions as well as specialized getters.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <iostream>

#include "../vertex.hpp"

using namespace wc3lib;

TEST_CASE("Vertex2dTest")
{
	Vertex2d<int32> vertex;

	REQUIRE(vertex.size() == 2);

	vertex[0] = 1;
	vertex[1] = 2;

	CHECK(vertex.x() == 1);
	CHECK(vertex.y() == 2);

	stringstream sstream(std::ios::in | std::ios::out | std::ios::binary);
	const std::streamsize writtenSize = vertex.write(sstream);
	const std::streamsize expectedSize = 2 * sizeof(int32) * sizeof(byte);

	REQUIRE(expectedSize == endPosition(sstream));
	REQUIRE(expectedSize == writtenSize);

	Vertex2d<int32> result;

	REQUIRE(result.size() == 2);

	sstream.seekg(0);

	REQUIRE(sstream.tellg() == 0);

	const std::streamsize readSize = result.read(sstream);

	REQUIRE(sstream);
	CHECK(readSize == expectedSize);
	CHECK(result == vertex);
	CHECK(result[0] == vertex[0]);
	CHECK(result[1] == vertex[1]);
}

TEST_CASE("Vertex3dTest")
{
	Vertex3d<int32> vertex;

	REQUIRE(vertex.size() == 3);

	vertex[0] = 1;
	vertex[1] = 2;
	vertex[2] = 3;

	CHECK(vertex.x() == 1);
	CHECK(vertex.y() == 2);
	CHECK(vertex.z() == 3);

	stringstream sstream(std::ios::in | std::ios::out | std::ios::binary);
	const std::streamsize writtenSize = vertex.write(sstream);
	const std::streamsize expectedSize = 3 * sizeof(int32) * sizeof(byte);

	REQUIRE(expectedSize == endPosition(sstream));
	REQUIRE(expectedSize == writtenSize);

	Vertex3d<int32> result;

	REQUIRE(result.size() == 3);

	sstream.seekg(0);

	REQUIRE(sstream.tellg() == 0);

	const std::streamsize readSize = result.read(sstream);

	CHECK(readSize == expectedSize);
	CHECK(result == vertex);
	CHECK(result[0] == vertex[0]);
	CHECK(result[1] == vertex[1]);
	CHECK(result[2] == vertex[2]);
}

TEST_CASE("QuaternionTest")
{
	Quaternion<int32> quaternion;

	REQUIRE(quaternion.size() == 4);

	quaternion[0] = 1;
	quaternion[1] = 2;
	quaternion[2] = 3;
	quaternion[3] = 4;

	CHECK(quaternion.a() == 1);
	CHECK(quaternion.b() == 2);
	CHECK(quaternion.c() == 3);
	CHECK(quaternion.d() == 4);

	stringstream sstream(std::ios::in | std::ios::out | std::ios::binary);
	const std::streamsize writtenSize = quaternion.write(sstream);
	const std::streamsize expectedSize = 4 * sizeof(int32) * sizeof(byte);

	REQUIRE(expectedSize == endPosition(sstream));
	REQUIRE(expectedSize == writtenSize);

	Quaternion<int32> result;

	REQUIRE(result.size() == 4);

	sstream.seekg(0);

	REQUIRE(sstream.tellg() == 0);

	const std::streamsize readSize = result.read(sstream);

	CHECK(readSize == expectedSize);
	CHECK(result == quaternion);
	CHECK(result[0] == quaternion[0]);
	CHECK(result[1] == quaternion[1]);
	CHECK(result[2] == quaternion[2]);
	CHECK(result[3] == quaternion[3]);
}
