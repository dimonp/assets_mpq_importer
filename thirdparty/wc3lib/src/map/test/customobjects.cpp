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

#include <boost/cast.hpp>
//#include <boost/foreach.hpp>

#include "../../platform.hpp"
#include "../customobjectscollection.hpp"

using namespace wc3lib;

/**
 * \file
 * For this Unit Test a custom objects file has been created with one custom object
 * per category with a changed named field only.
 */

struct MyConfig
{
	MyConfig()
	{
		INFO("global setup");

		std::ifstream in("customobjects.w3o", std::ifstream::in | std::ifstream::binary);

		if (in)
		{
			valid = true;

			try
			{
				collection.read(in);
			}
			catch (Exception &e)
			{
				valid = false;
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			valid = false;
		}
	}

	~MyConfig()
	{
		INFO("global teardown");

		collection.clear();
		valid = true;
	}

	static map::CustomObjectsCollection collection;
	static bool valid;
};

map::CustomObjectsCollection MyConfig::collection;
bool MyConfig::valid = true;

//BOOST_FIXTURE_TEST_SUITE(TestSuite, MyConfig)

TEST_CASE_METHOD(MyConfig, "Units", "[TestSuite]")
{
	REQUIRE(strcmp(MyConfig::collection.fileExtension(), ".w3o") == 0);
	REQUIRE(MyConfig::valid);
	REQUIRE(MyConfig::collection.hasUnits());
	REQUIRE(MyConfig::collection.units().get() != 0);
	REQUIRE(MyConfig::collection.units()->type() == map::CustomObjects::Type::Units);
	REQUIRE(strcmp(MyConfig::collection.units()->fileName(), "war3map.w3u") == 0);
	REQUIRE(MyConfig::collection.units()->customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&MyConfig::collection.units()->customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Units);
	REQUIRE(map::idToString(object->originalId()) == "hpea"); // footman id
	REQUIRE(map::idToString(object->customId()) == "h000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changed
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "unam"); // the values id are taken from "UnitMetaData.slk" first column
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Unit");
}

TEST_CASE_METHOD(MyConfig, "Items", "[TestSuite]")
{
	REQUIRE(strcmp(MyConfig::collection.fileExtension(), ".w3o") == 0);
	REQUIRE(MyConfig::valid);
	REQUIRE(MyConfig::collection.hasItems());
	REQUIRE(MyConfig::collection.items().get() != 0);
	REQUIRE(MyConfig::collection.items()->type() == map::CustomObjects::Type::Items);
	REQUIRE(strcmp(MyConfig::collection.items()->fileName(), "war3map.w3t") == 0);
	REQUIRE(MyConfig::collection.items()->customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&MyConfig::collection.items()->customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Items);
	REQUIRE(map::idToString(object->originalId()) == "tkno"); // tome of power
	REQUIRE(map::idToString(object->customId()) == "I000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changedl
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "unam"); // the values id are taken from "UnitMetaData.slk" first column
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Item");
}

TEST_CASE_METHOD(MyConfig, "Destructibles", "[TestSuite]")
{
	REQUIRE(strcmp(MyConfig::collection.fileExtension(), ".w3o") == 0);
	REQUIRE(MyConfig::valid);
	REQUIRE(MyConfig::collection.hasDestructibles());
	REQUIRE(MyConfig::collection.destructibles().get() != 0);
	REQUIRE(MyConfig::collection.destructibles()->type() == map::CustomObjects::Type::Destructibles);
	REQUIRE(strcmp(MyConfig::collection.destructibles()->fileName(), "war3map.w3b") == 0);
	REQUIRE(MyConfig::collection.destructibles()->customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&MyConfig::collection.destructibles()->customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Destructibles);
	REQUIRE(map::idToString(object->originalId()) == "DTrf"); // first destructible
	REQUIRE(map::idToString(object->customId()) == "B000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changedl
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "bnam"); // the values id are taken from "DestructableMetaData.slk" first column
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Destructible");
}

TEST_CASE_METHOD(MyConfig, "Doodads", "[TestSuite]")
{
	REQUIRE(strcmp(MyConfig::collection.fileExtension(), ".w3o") == 0);
	REQUIRE(MyConfig::valid);
	REQUIRE(MyConfig::collection.hasDoodads());
	REQUIRE(MyConfig::collection.doodads().get() != 0);
	REQUIRE(MyConfig::collection.doodads()->type() == map::CustomObjects::Type::Doodads);
	REQUIRE(strcmp(MyConfig::collection.doodads()->fileName(), "war3map.w3d") == 0);
	REQUIRE(MyConfig::collection.doodads()->customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&MyConfig::collection.doodads()->customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Doodads);
	REQUIRE(map::idToString(object->originalId()) == "ASbl"); // first doodad
	REQUIRE(map::idToString(object->customId()) == "D000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changedl
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "dnam"); // the values id are taken from "Doodads/DoodadMetaData.slk" first column
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Doodad");
}

TEST_CASE_METHOD(MyConfig, "Abilities", "[TestSuite]")
{
	REQUIRE(strcmp(MyConfig::collection.fileExtension(), ".w3o") == 0);
	REQUIRE(MyConfig::valid);
	REQUIRE(MyConfig::collection.hasAbilities());
	REQUIRE(MyConfig::collection.abilities().get() != 0);
	REQUIRE(MyConfig::collection.abilities()->type() == map::CustomObjects::Type::Abilities);
	REQUIRE(strcmp(MyConfig::collection.abilities()->fileName(), "war3map.w3a") == 0);
	REQUIRE(MyConfig::collection.abilities()->customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&MyConfig::collection.abilities()->customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Abilities);
	REQUIRE(map::idToString(object->originalId()) == "AUcb"); // first ability
	REQUIRE(map::idToString(object->customId()) == "A000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changedl
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "anam"); // the values id are taken from "AbilityMetaData.slk" first column
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Ability");
}

TEST_CASE_METHOD(MyConfig, "Buffs", "[TestSuite]")
{
	REQUIRE(strcmp(MyConfig::collection.fileExtension(), ".w3o") == 0);
	REQUIRE(MyConfig::valid);
	REQUIRE(MyConfig::collection.hasBuffs());
	REQUIRE(MyConfig::collection.buffs().get() != 0);
	REQUIRE(MyConfig::collection.buffs()->type() == map::CustomObjects::Type::Buffs);
	REQUIRE(strcmp(MyConfig::collection.buffs()->fileName(), "war3map.w3h") == 0);
	REQUIRE(MyConfig::collection.buffs()->customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&MyConfig::collection.buffs()->customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Buffs);
	REQUIRE(map::idToString(object->originalId()) == "BUcb"); // first ability
	REQUIRE(map::idToString(object->customId()) == "B000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changedl
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "fnam"); // the values id are taken from "Units\AbilityBuffMetaData.slk" first column NOTE the file is not listed in the listfile
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Buff");
}

TEST_CASE_METHOD(MyConfig, "Upgrades", "[TestSuite]")
{
	REQUIRE(strcmp(MyConfig::collection.fileExtension(), ".w3o") == 0);
	REQUIRE(MyConfig::valid);
	REQUIRE(MyConfig::collection.hasUpgrades());
	REQUIRE(MyConfig::collection.upgrades().get() != 0);
	REQUIRE(MyConfig::collection.upgrades()->type() == map::CustomObjects::Type::Upgrades);
	REQUIRE(strcmp(MyConfig::collection.upgrades()->fileName(), "war3map.w3q") == 0);
	REQUIRE(MyConfig::collection.upgrades()->customTable().size() == 1);
	const map::CustomObjects::Object *object = boost::polymorphic_cast<map::CustomObjects::Object*>(&MyConfig::collection.upgrades()->customTable().at(0));
	REQUIRE(object->type() == map::CustomObjects::Type::Upgrades);
	REQUIRE(map::idToString(object->originalId()) == "Rhme"); // first upgrade
	REQUIRE(map::idToString(object->customId()) == "R000");
	REQUIRE(object->sets().size() == 1);
	const map::CustomObjects::Set *set = boost::polymorphic_cast<const map::CustomObjects::Set*>(&object->sets().at(0));
	REQUIRE(set->modifications().size() == 1); // only name field changedl
	const map::CustomObjects::Modification *modification = boost::polymorphic_cast<const map::CustomObjects::Modification*>(&set->modifications().at(0));
	REQUIRE(map::idToString(modification->valueId()) == "gnam"); // the values id are taken from "UpgradeMetaData.slk" first column
	REQUIRE(modification->value().type() == map::Value::Type::String);
	REQUIRE(modification->value().toString() == "My Upgrade");
}

/*
 * Test whether the copy constructors and the polymorphic cloning works without crashes and losing data.
 */
TEST_CASE_METHOD(MyConfig, "Cloning", "[TestSuite]")
{
	REQUIRE(strcmp(MyConfig::collection.fileExtension(), ".w3o") == 0);
	REQUIRE(MyConfig::valid);
	REQUIRE(MyConfig::collection.hasUnits());
	REQUIRE(MyConfig::collection.units().get() != 0);
	REQUIRE(MyConfig::collection.units()->type() == map::CustomObjects::Type::Units);
	REQUIRE(strcmp(MyConfig::collection.units()->fileName(), "war3map.w3u") == 0);
	REQUIRE(MyConfig::collection.units()->customTable().size() == 1);

	map::CustomObjectsCollection clonedCollection;
	map::CustomObjects *units = new map::CustomObjects(*MyConfig::collection.units());
	REQUIRE(units != nullptr);
	clonedCollection.units().reset(units);
	REQUIRE(clonedCollection.hasUnits());
	REQUIRE(clonedCollection.units().get() != nullptr);
	REQUIRE(clonedCollection.units()->type() == map::CustomObjects::Type::Units);
	REQUIRE(strcmp(clonedCollection.units()->fileName(), "war3map.w3u") == 0);
	REQUIRE(clonedCollection.units()->customTable().size() == 1);
}

