/***************************************************************************
 *   Copyright (C) 2023 by Tamino Dauth                                    *
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
#include <boost/cast.hpp>

#include "../customobjectscollection.hpp"

using namespace wc3lib;

TEST_CASE("ok", "[customobjectswowr]")
{
	REQUIRE(true);
}

TEST_CASE("Units", "[.][customobjectswowr]")
{
	std::ifstream in("reforged/war3map.w3u", std::ifstream::in | std::ifstream::binary);

	map::CustomObjects customUnits(map::CustomObjects::Type::Units);
	customUnits.read(in);

	REQUIRE(customUnits.type() == map::CustomObjects::Type::Units);
	REQUIRE(strcmp(customUnits.fileName(), "war3map.w3u") == 0);
	REQUIRE(customUnits.customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&customUnits.customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Units);
	REQUIRE(map::idToString(object->originalId()) == "hpea"); // footman id
	REQUIRE(map::idToString(object->customId()) == "h000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changedl
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "unam"); // the values id are taken from "UnitMetaData.slk" first column
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Unit");
}

TEST_CASE("Abilities", "[.][customobjectswowr]")
{
	std::ifstream in("reforged/war3map.w3a", std::ifstream::in | std::ifstream::binary);

	map::CustomObjects customUnits(map::CustomObjects::Type::Abilities);
	customUnits.read(in);

	REQUIRE(customUnits.type() == map::CustomObjects::Type::Abilities);
	REQUIRE(strcmp(customUnits.fileName(), "war3map.w3a") == 0);
	REQUIRE(customUnits.customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&customUnits.customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Abilities);
	REQUIRE(map::idToString(object->originalId()) == "hpea"); // footman id
	REQUIRE(map::idToString(object->customId()) == "h000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changedl
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "unam"); // the values id are taken from "AbilityMetaData.slk" first column
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Unit");
}
