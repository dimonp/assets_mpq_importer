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

#include "../../platform.hpp"
#include "../sounds.hpp"

using namespace wc3lib;

TEST_CASE("ReadTest") {
	ifstream in("war3map.w3s", ifstream::in | ifstream::binary); // TestMap.w3m

	REQUIRE(in);

	map::Sounds sounds;

	bool valid = true;

	try {
		sounds.read(in);
	}
	catch (std::exception &e) {
		valid = false;
		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);

	REQUIRE(sounds.sounds().size() == 3);
	const map::Sound &sound = sounds.sounds().at(0);
	REQUIRE(sound.name() == "gg_snd_RainOfFireLoop1");
	REQUIRE(sound.file() == "Abilities\\Spells\\Demon\\RainOfFire\\RainOfFireLoop1.wav");
	REQUIRE(sound.usesDefaultVolume());
	REQUIRE(sound.flags() & map::Sound::Flags::StopWhenOutOfRange);
	REQUIRE(sound.flags() & map::Sound::Flags::Sound3d);
	REQUIRE(sound.eaxEffects() == map::Sound::EAX::Spells);
	REQUIRE((sound.channel() == map::Sound::Channel::Birth || sound.channel() == map::Sound::Channel::UseDefault));
	// TODO check all sounds and properties
}

TEST_CASE("ReadWriteReadTest") {
	ifstream in("war3map.w3s", istream::in | ifstream::binary); // Reign of Chaos

	REQUIRE(in);

	map::Sounds sounds;

	bool valid = true;

	try {
		sounds.read(in);
	}
	catch (std::exception &e) {
		valid = false;
		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);

	in.close();
	ofstream out("war3map.w3sout", ifstream::out | ifstream::binary);

	REQUIRE(out);

	try {
		sounds.write(out);
	}
	catch (std::exception &e) {
		valid = false;
		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);

	out.close(); // flush file stream
	sounds.sounds().clear(); // ensure it's empty!

	in.open("war3map.w3sout", ifstream::in | ifstream::binary); // Reign of Chaos, reopen

	REQUIRE(in);

	try {
		sounds.read(in);
	}
	catch (std::exception &e) {
		valid = false;
		std::cerr << e.what() << std::endl;
	}

	REQUIRE(valid);
	// TODO check all sounds and properties
}
