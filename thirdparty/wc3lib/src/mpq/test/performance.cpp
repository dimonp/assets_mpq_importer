/***************************************************************************
 *   Copyright (C) 2016 by Tamino Dauth                                    *
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
#include <chrono>

#include <filesystem>

/*
 * This test uses the StormLib to compare performances.
 */
#include <StormLib.h>
#include <StormPort.h>

#include "../archive.hpp"

using namespace wc3lib;


#ifdef WC3_DIR
/*
 * Reads all files from war3.mpq.
 *
 * This test compares the file read speed of wc3lib and StormLib.
 * Note that StormLib provides more efficient ways of reading the whole archive and iterating through all files than opening each file by its name.
 * Since the listfile entry extraction is excluded from the timer only the iterating blocks next to each other without massive seeking in the file would be much faster in StormLib than using this solution.
 */
TEST_CASE("ReadWar3", "[.][perfomance]")
{
	/*
	 * Copy original war3.mpq.
	 * Since the original war3.mpq may not be distributed due to licensing, this test can only be run if Warcraft III: Reign of Chaos is installed.
	 * The installation directory must be specified in "WC3_DIR" which is an environment variable.
	 */
	std::filesystem::path wc3DirPath = WC3_DIR;
	std::cerr << "wc3 dir" << wc3DirPath << std::endl;
	REQUIRE(std::filesystem::exists(wc3DirPath));
	REQUIRE(std::filesystem::is_directory(wc3DirPath));

	const std::filesystem::path war3Path = wc3DirPath / "War3.mpq";

	REQUIRE(std::filesystem::exists(war3Path));

	const  std::filesystem::path workingDir = std::filesystem::current_path();

	REQUIRE(std::filesystem::exists(workingDir));
	REQUIRE(std::filesystem::is_directory(workingDir));

	const  std::filesystem::path target = workingDir / "War3.mpq";

	/*
	 * Overwrite old copy to get a fresh unmodified file.
	 */
	if (std::filesystem::exists(target))
	{
		REQUIRE(std::filesystem::is_regular_file(target));
		REQUIRE(std::filesystem::remove(target));
	}

	std::cerr << "Copying " << war3Path << " to " << target << std::endl;

	std::error_code ec;
	std::filesystem::copy(war3Path, target, ec);

	std::cerr << "Error code: " << ec.message() << std::endl;

	REQUIRE(!ec);

	mpq::Archive archive;
	archive.open(target);

	REQUIRE(archive.isOpen());
	REQUIRE(archive.containsListfileFile());

	std::stringstream tmpStream0;
	const mpq::Listfile::Entries entries = archive.listfileFile().entries();
	std::size_t count0 = 0;
	const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	/*
	for (mpq::Listfile::Entries::const_iterator iterator = entries.begin(); iterator != entries.end(); ++iterator)
	{
		if (!iterator->empty())
		{
			mpq::File file = archive.findFile(*iterator);

			std::cerr << "File: " << *iterator << std::endl;

			REQUIRE(file.isValid());

			file.writeData(tmpStream0);

			++count0;
		}
	}
	*/

	const std::chrono::system_clock::time_point later = std::chrono::system_clock::now();
	const std::chrono::system_clock::duration duration = later - now;

	archive.close();

	std::cerr << "Duration wc3lib: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms" << std::endl;

	// TODO test with StormLib to compare results
	HANDLE stormArchive;
	REQUIRE(SFileOpenArchive(target.c_str(), 0, MPQ_FLAG_READ_ONLY, &stormArchive));
	std::size_t count1 = 0;
	std::stringstream tmpStream1;
	const std::chrono::system_clock::time_point now1 = std::chrono::system_clock::now();

	for (mpq::Listfile::Entries::const_iterator iterator = entries.begin(); iterator != entries.end(); ++iterator)
	{
		if (!iterator->empty())
		{
			if (SFileHasFile(stormArchive, iterator->c_str()))
			{
				std::cerr << "File: " << *iterator << std::endl;

				HANDLE file;
				REQUIRE(SFileOpenFileEx(stormArchive, iterator->c_str(), 0, &file));
				REQUIRE(file != 0);
				DWORD fileSize = 0;
				DWORD fileSizeHigh = 0;
				fileSize = SFileGetFileSize(file, &fileSizeHigh);

				std::cerr << "File size: " << fileSize << " file size high: " << fileSizeHigh << std::endl;

				REQUIRE(fileSize > 0);

				char *buffer = new char[fileSize];
				REQUIRE(buffer != nullptr);
				char *overlapped = nullptr;
				REQUIRE(SFileReadFile(file, buffer, fileSize, &fileSizeHigh, overlapped));

				//tmpStream1.write(reinterpret_cast<const byte*>(buffer), fileSize);
				// TODO write file size high and overlapped buffer.

				delete [] buffer;

				if (overlapped != nullptr)
				{
					delete overlapped;
				}

				REQUIRE(SFileCloseFile(file));

				++count1;
			}
		}
	}
	REQUIRE(SFileCloseArchive(stormArchive));

	const std::chrono::system_clock::time_point later1 = std::chrono::system_clock::now();
	const std::chrono::system_clock::duration duration1 = later1 - now1;

	std::cerr << "Duration StormLib: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration1).count() << " ms" << std::endl;

	REQUIRE(count0 == count1);

}

#endif
