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

#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "../../spirit.hpp" // enable debug mode

#include "../../platform.hpp"
#include "../triggers.hpp"

using namespace wc3lib;

TEST_CASE("TriggerDataReignOfChaos", "[triggers]")
{
	ifstream in("TriggerData.txt"); // Warcraft III trigger data

	REQUIRE(in);

	map::TriggerData triggerData;

	REQUIRE_NOTHROW(triggerData.read(in));

	in.close();

	CHECK(strcmp(triggerData.fileName(), "TriggerData.txt") == 0);
	CHECK(triggerData.version() == 0); // has no specific version
	CHECK(triggerData.latestFileVersion() == 0); // has no specific version

	// data from the file
	CHECK(triggerData.categories().size() == 43); // section [TriggerCategories]
	REQUIRE(triggerData.types().size() == 133); // section [TriggerTypes]
	// TODO check more sections

	// first type:
	// abilcode=1,1,WESTRING_TRIGTYPE_abilcode,integer
	// const map::TriggerData::Type &firstType = *triggerData.types().cbegin()->second;
	// CHECK(firstType.name() == "abilcode");
	// CHECK(firstType.canBeGlobal());
	// CHECK(firstType.canBeCompared());
	// CHECK(firstType.displayText() == "WESTRING_TRIGTYPE_abilcode");
	// CHECK(firstType.baseType() != nullptr);
	// CHECK(firstType.baseType()->name() == "integer");
	// CHECK(firstType.defaultValue() == "");

	/*
	[DefaultTriggerCategories]
	// Defines categories to be automatically added to new maps
	NumCategories=1

	Category01=WESTRING_INITIALIZATION

	[DefaultTriggers]
	// Defines triggers to be automatically added to new maps
	NumTriggers=1

	// Melee Initialization
	Trigger01Name=WESTRING_MELEEINITIALIZATION
	Trigger01Comment=WESTRING_MELEEINITIALIZATION_COMMENT
	Trigger01Category=1
	Trigger01Events=1
	Trigger01Event01=MapInitializationEvent
	Trigger01Conditions=0
	Trigger01Actions=8
	Trigger01Action01=MeleeStartingVisibility
	Trigger01Action02=MeleeStartingHeroLimit
	Trigger01Action03=MeleeGrantHeroItems
	Trigger01Action04=MeleeStartingResources
	Trigger01Action05=MeleeClearExcessUnits
	Trigger01Action06=MeleeStartingUnits
	Trigger01Action07=MeleeStartingAI
	Trigger01Action08=MeleeInitVictoryDefeat
	*/
	// there is one default trigger for melee maps
	REQUIRE(triggerData.defaultTriggerCategories().size() == 1);
	REQUIRE(triggerData.defaultTriggers().size() == 1);

	map::TriggerData::DefaultTriggerCategories::const_reference defaultCategory = triggerData.defaultTriggerCategories().front();
	CHECK(defaultCategory == "WESTRING_INITIALIZATION");

	map::TriggerData::DefaultTriggers::const_reference defaultTrigger = triggerData.defaultTriggers().front();
	CHECK(defaultTrigger.name() == "WESTRING_MELEEINITIALIZATION");
	CHECK(defaultTrigger.comment() == "WESTRING_MELEEINITIALIZATION_COMMENT");
	CHECK(defaultTrigger.triggerCategory() == "WESTRING_INITIALIZATION");
	REQUIRE(defaultTrigger.events().size() == 1);
	REQUIRE(defaultTrigger.conditions().size() == 0);
	REQUIRE(defaultTrigger.actions().size() == 8);
}

TEST_CASE("TriggerDataReignOfChaosReadWriteRead", "[triggers]")
{
	spiritTraceLog.close();
	spiritTraceLog.open("triggerdata_reign_of_chaos_traces.xml");

	REQUIRE(spiritTraceLog);

	ifstream in("TriggerData.txt"); // Warcraft III trigger data

	REQUIRE(in);

	map::TriggerData triggerData;

	REQUIRE_NOTHROW(triggerData.read(in));

	in.close();

	CHECK(strcmp(triggerData.fileName(), "TriggerData.txt") == 0);
	CHECK(triggerData.version() == 0); // has no specific version
	CHECK(triggerData.latestFileVersion() == 0); // has no specific version

	// data from the file
	CHECK(triggerData.categories().size() == 43); // section [TriggerCategories]
	CHECK(triggerData.types().size() == 133); // section [TriggerTypes]
	// TODO check more sections

	// there is one default trigger for melee maps
	CHECK(triggerData.defaultTriggerCategories().size() == 1);
	CHECK(triggerData.defaultTriggers().size() == 1);

	spiritTraceLog.close();
	spiritTraceLog.open("triggerdata_reign_of_chaos_tmp_out_traces.xml");

	REQUIRE(spiritTraceLog);

	ofstream out("TriggerDataTmp.txt");

	REQUIRE(out);
	REQUIRE_NOTHROW(triggerData.write(out));

	out.close();
	spiritTraceLog.close();
	spiritTraceLog.open("triggerdata_reign_of_chaos_tmp_traces.xml");

	REQUIRE(spiritTraceLog);

	in.open("TriggerDataTmp.txt");

	REQUIRE(in);

	triggerData.clear();

	REQUIRE(triggerData.categories().empty());
	REQUIRE(triggerData.types().empty());
	REQUIRE(triggerData.defaultTriggerCategories().empty());
	REQUIRE(triggerData.defaultTriggers().empty());

	// TODO check more sections

	// REQUIRE_NOTHROW(triggerData.read(in));

	// in.close();

	// CHECK(strcmp(triggerData.fileName(), "TriggerData.txt") == 0);
	// CHECK(triggerData.version() == 0); // has no specific version
	// CHECK(triggerData.latestFileVersion() == 0); // has no specific version

	// // data from the file
	// CHECK(triggerData.categories().size() == 43); // section [TriggerCategories]
	// CHECK(triggerData.types().size() == 133); // section [TriggerTypes]
	// // TODO check more sections

	// // there is one default trigger for melee maps
	// CHECK(triggerData.defaultTriggerCategories().size() == 1);
	// CHECK(triggerData.defaultTriggers().size() == 1);
}

TEST_CASE("WarChasersTriggersSimpleReadTest", "[triggers]")
{
	ifstream in("TriggerData.txt"); // Warcraft III trigger data

	REQUIRE(in);

	map::TriggerData triggerData;

	REQUIRE_NOTHROW(triggerData.read(in));

	in.close();

	ifstream inTriggers("war3map.wtg", std::ios::in | std::ios::binary); // War Chasers triggers
	map::Triggers triggers;

	REQUIRE_NOTHROW(triggers.read(inTriggers, triggerData));

	CHECK(strcmp(triggers.fileName(), "war3map.wtg") == 0);
	CHECK(memcmp(triggers.fileTextId(), "WTG!", 4) == 0);
	CHECK(triggers.version() == 4); // Reign of Chaos

	// data from the object manager
	CHECK(triggers.variables().size() == 54);
	CHECK(triggers.triggers().size() == 151);
	CHECK(triggers.categories().size() == 19);

	// TODO check single trigger
}

TEST_CASE("WarChasersTriggersSimpleReadTestWithFrozenThroneTriggerData", "[.][triggers]")
{
	ifstream in("TriggerDataEx.txt"); // Warcraft III: The Frozen Throne trigger data

	REQUIRE(in);

	map::TriggerData triggerData;

	REQUIRE_NOTHROW(triggerData.read(in));

	in.close();

	ifstream inTriggers("war3map.wtg", std::ios::in | std::ios::binary); // War Chasers triggers
	map::Triggers triggers;

	REQUIRE_NOTHROW(triggers.read(inTriggers, triggerData));

	CHECK(strcmp(triggers.fileName(), "war3map.wtg") == 0);
	CHECK(memcmp(triggers.fileTextId(), "WTG!", 4) == 0);
	CHECK(triggers.version() == 4); // Reign of Chaos

	// data from the object manager
	CHECK(triggers.variables().size() == 54);
	CHECK(triggers.triggers().size() == 151);
	CHECK(triggers.categories().size() ==19);

	// TODO check single trigger
}
