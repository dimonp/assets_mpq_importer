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

#include <iostream>
#include <filesystem>
#include <catch2/catch_test_macros.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

#include "../archive.hpp"
#include "../algorithm.hpp"

using namespace wc3lib;
using namespace wc3lib::mpq;

/**
 * Constructs a basic MPQ archive with one block and one hash entry (basically one file) and tests the basic IO methods of the class Archive.
 */
TEST_CASE("WriteReadHeaderAndTables", "[archive]")
{
	const std::size_t fileSize = 5;

	/*
	 * Initialize a header with one block table entry and one hash table entry.
	 * The block has a file size of 5 bytes.
	 */
	Header header;
	memcpy(header.magic, Archive::identifier, 4);
	header.headerSize = sizeof(header);
	header.archiveSize = sizeof(header) + sizeof(BlockTableEntry) + sizeof(HashTableEntry) + fileSize;
	header.formatVersion = static_cast<uint16>(Archive::Format::Mpq1);
	header.sectorSizeShift = 4096;
	header.hashTableOffset = sizeof(header) + sizeof(BlockTableEntry);
	header.blockTableOffset = sizeof(header);
	header.hashTableEntries = 1;
	header.blockTableEntries = 1;

	/*
	 * The block starts after the hash table entries.
	 */
	BlockTableEntry blockTableEntry;
	blockTableEntry.blockOffset = sizeof(header) + sizeof(BlockTableEntry) + sizeof(HashTableEntry);
	blockTableEntry.blockSize = fileSize;
	blockTableEntry.fileSize = fileSize;
	blockTableEntry.flags = static_cast<uint32>(Block::Flags::IsFile);

	/*
	 * The hash table entry belongs to the only block table entry.
	 */
	HashTableEntry hashTableEntry;
	hashTableEntry.filePathHashA = static_cast<int32>(HashString(Archive::cryptTable(), "test.txt", HashType::NameA));
	hashTableEntry.filePathHashB = static_cast<int32>(HashString(Archive::cryptTable(), "test.txt", HashType::NameB));
	hashTableEntry.locale = static_cast<uint16>(File::Locale::Neutral);
	hashTableEntry.platform = static_cast<uint16>(File::Platform::Default);
	hashTableEntry.fileBlockIndex = 0;


	/*
	 * The custom MPQ archive is written into the file "tmp.mpq".
	 */
	std::ofstream out("tmp.mpq");
	boost::scoped_array<char> empty(new char[10]);
	memset(static_cast<void*>(empty.get()), 0, 10);
	out.write(static_cast<const char*>(empty.get()), 10); // skip 10 bytes and leave the file empty here
	// write the header structure which is not encrypted
	out.write(reinterpret_cast<const char*>(&header), sizeof(header));

	// write encrypted block table
	const std::size_t encryptedBytesSize = sizeof(struct BlockTableEntry);
	boost::scoped_array<byte> encryptedBytes(new byte[encryptedBytesSize]);
	memcpy(encryptedBytes.get(), &blockTableEntry, encryptedBytesSize);
	const uint32 hashValue = HashString(Archive::cryptTable(), "(block table)", HashType::FileKey);
	EncryptData(Archive::cryptTable(), encryptedBytes.get(), encryptedBytesSize, hashValue);
	out.write(encryptedBytes.get(), encryptedBytesSize);

	// write encrypted hash table
	const std::size_t encryptedBytesSizeHash = sizeof(struct HashTableEntry);
	boost::scoped_array<byte> encryptedBytesHash(new byte[encryptedBytesSizeHash]);
	memcpy(encryptedBytesHash.get(), &hashTableEntry, encryptedBytesSizeHash);
	const uint32 hashValueHash = HashString(Archive::cryptTable(), "(hash table)", HashType::FileKey);
	EncryptData(Archive::cryptTable(), encryptedBytesHash.get(), encryptedBytesSizeHash, hashValueHash);
	out.write(encryptedBytesHash.get(), encryptedBytesSizeHash);

	// write the file block sector
	boost::scoped_array<char> fileData(new byte[fileSize]);
	memset(fileData.get(), 0, fileSize);
	out.write(fileData.get(), fileSize);
	out.close();

	mpq::Archive archive;

	// check empty initialized values
	REQUIRE(archive.startPosition() == 0);
	REQUIRE(archive.blockTableOffset() == 0);
	REQUIRE(archive.extendedBlockTableOffset() == 0);
	REQUIRE(archive.hashTableOffset() == 0);
	REQUIRE(archive.strongDigitalSignaturePosition() == 0);
	REQUIRE(archive.blocks().empty());
	REQUIRE(archive.hashes().empty());
	REQUIRE(archive.sectorSize() == 0);
	REQUIRE(!archive.containsListfileFile());
	REQUIRE(!archive.containsAttributesFile());
	REQUIRE(!archive.containsSignatureFile());
	REQUIRE(!archive.hasStrongDigitalSignature());
	REQUIRE(!archive.isOpen());

	// load the header structure from the archive
	Header loadedHeader;

	ifstream in("tmp.mpq", std::ios::in | std::ios::binary);
	REQUIRE(in);
	uint64 startPosition = 0;
	std::streamsize size = 0;
	REQUIRE(archive.readHeader(in, loadedHeader, startPosition, size));
	REQUIRE(startPosition == 10);
	REQUIRE(size == sizeof(header));
	// compare everything with header
	REQUIRE(memcmp(&loadedHeader, &header, sizeof(loadedHeader)) == 0);

	// fix archive's offsets for the usage of the read methods since they use these values to seek to the positions in the stream
	archive.setStartPosition(startPosition);
	archive.setBlockTableOffset(loadedHeader.blockTableOffset);
	// TODO extended block table offset?
	archive.setHashTableOffset(loadedHeader.hashTableOffset);

	// compare everything with block entry
	size = 0;
	REQUIRE(archive.readBlockTable(in, 1, size));
	REQUIRE(size == sizeof(blockTableEntry));
	REQUIRE(archive.blocks().size() == 1);
	const Block &loadedBlock = archive.blocks()[0];
	const BlockTableEntry loadedBlockTableEntry = loadedBlock.toBlockTableEntry();
	REQUIRE(loadedBlockTableEntry.blockOffset == blockTableEntry.blockOffset);
	// the loaded block table entry has to be the exact same as the written
	REQUIRE(memcmp(&loadedBlockTableEntry, &blockTableEntry, sizeof(blockTableEntry)) == 0);

	// check methods of class Block
	REQUIRE(!loadedBlock.empty());
	// TODO check more

	// compare everything with hash entry
	size = 0;
	REQUIRE(archive.readHashTable(in, 1, size));
	REQUIRE(size == sizeof(hashTableEntry));
	REQUIRE(archive.hashes().size() == 1);
	const Hash &loadedHash = *archive.hashes().begin()->second;
	const HashTableEntry loadedHashTableEntry = loadedHash.toHashTableEntry();
	// the loaded hash table entry has to be the exact same as the written
	REQUIRE(memcmp(&loadedHashTableEntry, &hashTableEntry, sizeof(hashTableEntry)) == 0);

	// check methods of class Hash
	REQUIRE(!loadedHash.empty());
	REQUIRE(!loadedHash.deleted());
	REQUIRE(loadedHash.block() == &loadedBlock);
	REQUIRE(loadedHash.index() == 0);
	// TODO check more
}

TEST_CASE("ReadMPQMasterArchive", "[archive]")
{
	mpq::Archive archive;
	REQUIRE(archive.open("test_with_no_extended_attributes.mpq") > 0);
	// TODO find file
}

#ifdef WC3_DIR

TEST_CASE("ReadWar3Archive", "[archive]")
{
	std::filesystem::path wc3DirPath = WC3_DIR;
	std::cerr << "wc3 dir" << wc3DirPath << std::endl;
	REQUIRE(std::filesystem::exists(wc3DirPath));
	REQUIRE(std::filesystem::is_directory(wc3DirPath));

	const std::filesystem::path war3Path = wc3DirPath / "War3.mpq";

	REQUIRE(std::filesystem::exists(war3Path));

	Archive war3Archive;
	REQUIRE(war3Archive.open(war3Path) > 0);
	REQUIRE(war3Archive.format() == Archive::Format::Mpq1);
	REQUIRE(war3Archive.containsListfileFile());

	const File listfileFile = war3Archive.findFile("(listfile)");
	REQUIRE(listfileFile.isValid());
}

TEST_CASE("ReadWar3XArchive", "[archive]")
{
	std::filesystem::path wc3DirPath = WC3_DIR;
	std::cerr << "wc3 dir" << wc3DirPath << std::endl;
	REQUIRE(std::filesystem::exists(wc3DirPath));
	REQUIRE(std::filesystem::is_directory(wc3DirPath));

	const std::filesystem::path war3XPath = wc3DirPath / "War3x.mpq";

	REQUIRE(std::filesystem::exists(war3XPath));

	Archive war3XArchive;
	REQUIRE(war3XArchive.open(war3XPath) > 0);
	REQUIRE(war3XArchive.format() == Archive::Format::Mpq1);
	REQUIRE(war3XArchive.containsListfileFile());

	const File listfileFile = war3XArchive.findFile("(listfile)");
	REQUIRE(listfileFile.isValid());
}

#endif

TEST_CASE("CreateEmptyArchive", "[archive]")
{
	Archive archive;

	REQUIRE(archive.create("emptyarchive.mpq", 1, 1) > 0);

	CHECK(archive.isOpen());

	CHECK(archive.blocks().size() == 1);
	CHECK(archive.hashes().size() == 1);

	CHECK(std::filesystem::is_regular_file("emptyarchive.mpq"));

	const std::size_t expectedArchiveSize = sizeof(Header) + sizeof(BlockTableEntry) + sizeof(HashTableEntry); // size includes the size of the header
	CHECK(archive.size() == expectedArchiveSize);

	// now close and reopen it and check if the data is still valid
	archive.close();

	CHECK(!archive.isOpen());

	archive.open("emptyarchive.mpq");

	REQUIRE(archive.isOpen());
	CHECK(archive.blocks().size() == 1);
	CHECK(archive.hashes().size() == 1);
	CHECK(archive.size() == expectedArchiveSize);
}

TEST_CASE("AddFileUncompressed", "[archive]")
{
	if (std::filesystem::exists("addfile.mpq"))
	{
		std::filesystem::remove("addfile.mpq");
	}

	Archive archive;
	archive.create("addfile.mpq", 1, 1);

	REQUIRE(archive.isOpen());
	REQUIRE(archive.blocks().size() == 1);
	REQUIRE(archive.hashes().size() == 1);

	string data = "Hello World!";

	File file = archive.addFile("test.txt", data.c_str(), data.size());

	REQUIRE(file.isValid());

	archive.close();

	// make sure the archive has been written
	REQUIRE(std::filesystem::exists("addfile.mpq"));

	archive.open("addfile.mpq");

	REQUIRE(archive.isOpen());
	REQUIRE(archive.blocks().size() == 1);
	REQUIRE(archive.hashes().size() == 1);

	file = archive.findFile("test.txt");

	REQUIRE(file.isValid());

	std::stringstream sstream;
	file.decompress(sstream);
	data = sstream.str();

	REQUIRE(data == "Hello World!");
}

TEST_CASE("AddFileUncompressedEncrypted", "[archive]")
{
	if (std::filesystem::exists("addfile.mpq"))
	{
		std::filesystem::remove("addfile.mpq");
	}

	Archive archive;
	archive.create("addfile.mpq", 1, 1);

	REQUIRE(archive.isOpen());
	REQUIRE(archive.blocks().size() == 1);
	REQUIRE(archive.hashes().size() == 1);

	std::string data = "Hello World!";

	File file = archive.addFile("test.txt", data.c_str(), data.size(), Sector::Compression::Uncompressed, Block::Flags::IsEncrypted);

	REQUIRE(file.isValid());

	archive.close();

	// make sure the archive has been written
	REQUIRE(std::filesystem::exists("addfile.mpq"));

	archive.open("addfile.mpq");

	REQUIRE(archive.isOpen());
	REQUIRE(archive.blocks().size() == 1);
	REQUIRE(archive.hashes().size() == 1);

	file = archive.findFile("test.txt");

	REQUIRE(file.isValid());

	std::stringstream sstream;
	file.decompress(sstream);
	data = sstream.str();

	REQUIRE(data == "Hello World!");
}

TEST_CASE("RemoveFile", "[archive]")
{
	if (std::filesystem::exists("removefile.mpq"))
	{
		std::filesystem::remove("removefile.mpq");
	}

	Archive archive;
	archive.create("removefile.mpq", 1, 1);

	REQUIRE(archive.isOpen());
	REQUIRE(archive.blocks().size() == 1);
	REQUIRE(archive.hashes().size() == 1);

	std::string data = "Hello World!";

	File file = archive.addFile("test.txt", data.c_str(), data.size());

	REQUIRE(file.isValid());

	archive.close();

	// make sure the archive has been written
	REQUIRE(std::filesystem::exists("removefile.mpq"));

	archive.open("removefile.mpq");

	REQUIRE(archive.isOpen());
	REQUIRE(archive.blocks().size() == 1);
	REQUIRE(archive.hashes().size() == 1);

	file = archive.findFile("test.txt");

	REQUIRE(file.isValid());

	std::stringstream sstream;
	file.decompress(sstream);
	data = sstream.str();

	REQUIRE(data == "Hello World!");

	REQUIRE(archive.removeFile(file));

	// the entries are still there but deleted
	REQUIRE(archive.blocks().size() == 1);
	REQUIRE(archive.hashes().size() == 1);
	const Block &block = archive.blocks()[0];
	const Hash &hash = *archive.hashes().begin()->second;

	REQUIRE(block.empty());
	REQUIRE(hash.empty());

	// the file is not in the archive anymore
	file = archive.findFile("test.txt");

	REQUIRE(!file.isValid());
}
