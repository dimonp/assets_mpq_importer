/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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

#include "file.hpp"
#include "hash.hpp"
#include "archive.hpp"
#include "algorithm.hpp"

namespace wc3lib::mpq
{

std::streamsize File::compress(istream &istream, Sector::Compression compression)
{
	throw Exception(_("Not supported yet!"));
}

std::streamsize File::decompress(ostream &ostream) const
{
	ifstream ifstream(this->archive()->path(), std::ios_base::in | std::ios_base::binary);

	if (!ifstream)
	{
		throw Exception(std::format(_("Unable to open file {}."), this->archive()->path().string()));
	}

	return this->decompress(ifstream, ostream);
}

std::streamsize File::decompress(istream &istream, ostream &ostream) const
{
	/*
	 * Read sectors from MPQ archive file and store them.
	 */
	Sector::Sectors sectors;
	this->sectors(istream, sectors);

	if (sectors.empty() && hasSectorOffsetTable() && isEncrypted() && path().empty())
	{
		std::cerr << std::format(_("Warning: Writing data from file with block index {} and hash index {} failed because we need its path to decrypt its data."), 
			block()->index(), hash()->index()) << std::endl;

		return 0;
	}

	std::streamsize bytes = 0;

	for(Sector::Sectors::const_reference sector : sectors)
	{
		try
		{
			sector.seekg(istream);
			bytes += sector.decompress(istream, ostream);
		}
		catch (Exception &exception)
		{
			throw Exception(std::format(_("Sector error (sector {}, file {}):\n{}"), 
				sector.sectorIndex(), this->path().string(), exception.what()));
		}
	}

	return bytes;
}

File::Locale File::locale() const
{
	return File::intToLocale(this->hash()->cHashData().locale());
}

File::Platform File::platform() const
{
	return File::intToPlatform(this->hash()->cHashData().platform());
}

Block* File::block() const
{
	return this->hash()->block();
}

File::File() : m_archive(0), m_hash(0)
{
}

void File::close()
{
	this->m_archive = 0;
	this->m_hash = 0;
}

File::File(Archive *archive, Hash *hash, const std::filesystem::path &path) : m_archive(archive), m_hash(hash), m_path(path)
{
}

File::~File()
{
}

File::File(const File &other) : m_archive(other.archive()), m_hash(other.hash()), m_path(other.path())
{
}

std::streamsize File::sectors(istream &istream, Sector::Sectors &sectors) const
{
	// if we have a sector offset table and file is encrypted we first need to know its path for proper decryption!
	if (hasSectorOffsetTable() && isEncrypted() && path().empty())
	{
		return 0;
	}

	istream.seekg(archive()->startPosition());
	istream.seekg(this->block()->blockOffset(), std::ios::cur);

	if (archive()->format() == Archive::Format::Mpq2 && block()->extendedBlockOffset() > 0)
	{
		istream.seekg(block()->extendedBlockOffset(), std::ios::cur);
	}

	/*
	 * The filename is required for sectors which are encrypted to decrypt them using the file's key + the sector index.
	 * The file key is calculated using the filename only without any directory path.
	 */
	const string fileName = this->name();

	std::streamsize bytes = 0;

	// This table is not present if this information can be calculated.
	// If the file is not compressed/imploded, then the size and offset of all sectors is known, based on the archive's SectorSizeShift. If the file is stored as a single unit compressed/imploded, then the SectorOffsetTable is omitted, as the single file "sector" corresponds to BlockSize and FileSize, as mentioned previously.
	if (!this->isCompressed() && !this->isImploded())
	{
		if (this->size() != this->compressedSize())
		{
                        std::cerr << std::format(_("Although file {} is not compressed or imploded, compressed size {} differs from uncompressed size {}."), 
							this->path().string(), this->compressedSize(), this->size()) << std::endl;
		}

		const uint32 sectorSize = this->archive()->sectorSize();
		const uint32 sectorsCount = this->compressedSize() / sectorSize;
		sectors.reserve(sectorsCount + 1); // the last sector might use less size than the default one so reserve + 1

		uint32 newOffset = 0;
		// The sectors are not compressed so the size of each sector is the same in both cases.
		uint32 remainingUncompressedSize = this->size();

		for (uint32 i = 0; i < sectorsCount; ++i)
		{
			sectors.push_back(Sector(this->archive(), this->block(), fileName, i, newOffset, sectorSize, sectorSize));

			newOffset += sectorSize;
			remainingUncompressedSize -= sectorSize;
		}

		// the last sector might use less size (remaining bytes)
		if (remainingUncompressedSize > 0)
		{
			sectors.push_back(Sector(this->archive(), this->block(), fileName, sectors.size(), newOffset, remainingUncompressedSize, remainingUncompressedSize));
		}
	}
	// a single unit file only has one sector
	else if ((this->block()->flags() & Block::Flags::IsSingleUnit) && (this->isCompressed() || this->isImploded()))
	{
		const uint32 sectorOffset = 0;
		const uint32 compressedSectorSize = this->block()->blockSize();
		const uint32 uncompressedSectorSize = this->block()->fileSize();
		sectors.push_back(Sector(this->archive(), this->block(), fileName, 0, sectorOffset, compressedSectorSize, uncompressedSectorSize));
	}
	// However, the SectorOffsetTable will be present if the file is compressed/imploded and the file is not stored as a single unit, even if there is only a single sector in the file (the size of the file is less than or equal to the archive's sector size).
	// sector offset table
	else
	{
		const uint32 sectorsCount = this->block()->fileSize() / this->archive()->sectorSize() + (this->block()->fileSize() % this->archive()->sectorSize() > 0 ? 1 : 0);

		// last offset contains file size
		const uint32 offsetsSize = sectorsCount + 1;
		boost::scoped_array<uint32> offsets(new uint32[offsetsSize]);
		const uint32 readSize = offsetsSize * sizeof(uint32);
		wc3lib::read(istream, offsets[0], bytes, readSize);

		// The SectorOffsetTable, if present, is encrypted using the key - 1.
		if (isEncrypted())
		{
			DecryptData(Archive::cryptTable(), offsets.get(), readSize, fileKey() - 1);
		}

		sectors.reserve(sectorsCount);
		// the actual compressed file size, starting from the first offset of data
		//const uint32 size = offsets[sectorsCount] - offsets[0];
		// The remaining uncompressed size of the file
		uint32 remainingUncompressedSize = this->size();

		// if (offsets[0] > 0)
		// {
		// 	std::cerr << std::format(_("{} useless bytes in file {}."), offsets[0], this->path()) << std::endl;
		// }

		for (uint32 i = 0; i < sectorsCount; ++i)
		{
			/*
			 * The last entry contains the total compressed file
			 * size, making it possible to easily calculate the size of any given
			 * sector by simple subtraction.
			 */
			const uint32 sectorSize = offsets[i + 1] - offsets[i]; // works since last offset contains file size
			const uint32 uncompressedSectorSize = archive()->sectorSize() > remainingUncompressedSize ? remainingUncompressedSize : this->archive()->sectorSize(); // use remaining size of the block if it is smaller than the default expected uncompressed sector size

			sectors.push_back(Sector(this->archive(), this->block(), fileName, i, offsets[i], sectorSize, uncompressedSectorSize));

			remainingUncompressedSize -= uncompressedSectorSize;
		}
	}

	return bytes;
}

void File::changePath(const std::filesystem::path &path)
{
	this->m_path = path;

	if (this->hash() != 0)
	{
		this->hash()->changePath(path);
	}
}

std::streamsize File::sectors(Sector::Sectors &sectors) const
{
	ifstream istream(archive()->path(), std::ios::in | std::ios::binary);

	if (!istream)
	{
		throw Exception(std::format(_("Unable to open file {}."), archive()->path().string()));
	}

	std::streamsize result = this->sectors(istream, sectors);

	return result;
}

void File::removeData()
{
	throw Exception(_("Not implemented yet."));
}

bool File::hasSectorOffsetTable() const
{
	return Block::hasSectorOffsetTable(this->block()->flags());
}

}
