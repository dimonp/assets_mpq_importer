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

#include "mdlx.hpp"

namespace wc3lib::mdlx
{

const long32 Mdlx::tftVersion = 800;
const long32 Mdlx::reforgedVersion = 901;
const long32 Mdlx::currentVersion = 800;

Mdlx::Mdlx() : m_modelVersion(currentVersion)
{
}

// Based on information from https://www.hiveworkshop.com/threads/mdx-specifications.240487/
std::streamsize Mdlx::read(InputStream &istream)
{
	std::streamsize size = 0;
	char8_t tag[MDX_TAG_SIZE];
	wc3lib::read(istream, tag, size);
	expectMdxTag(istream, tag, u8"MDLX");

	auto p = istream.tellg();
	istream.seekg(0, std::ios::end);
	auto end = istream.tellg();
	istream.seekg(p);

	while (istream && istream.tellg() < end && remainingInputStreamSize(istream) >= sizeof(MdxChunkHeader))
	{
		p = istream.tellg();
		MdxChunkHeader chunkHeader;
		wc3lib::read(istream, chunkHeader, size);

#ifdef DEBUG
		std::cerr << "Found tag " << chunkHeader.readableTag() << " at position " << p << std::endl;
#endif

		if (isMdxTag(chunkHeader.tag, u8"VERS"))
		{
#ifdef DEBUG
			std::cerr << "Before reading VERS with previous position " << p << std::endl;
#endif
			std::streamsize modelVersionSize = 0;
			wc3lib::read(istream, m_modelVersion, modelVersionSize);
			skipMdxHeaderEmptyBytes(istream, chunkHeader, modelVersionSize);
			size += modelVersionSize;
		}
		else if (isMdxTag(chunkHeader.tag, u8"MODL"))
		{
#ifdef DEBUG
			std::cerr << "MODL" << std::endl;
			std::cerr << "Before reading MODL with previous position " << p << std::endl;
#endif
			istream.seekg(p);
			size += m_model.read(istream);
		}
		else if (isMdxTag(chunkHeader.tag, u8"SEQS"))
		{
#ifdef DEBUG
			std::cerr << "SEQS" << std::endl;
#endif
			m_sequences.clear();

			while (chunkHeader.size > 0)
			{
				Sequence sequence;
				const std::streamsize s = sequence.read(istream);
				chunkHeader.size -= s;
				size += s;
				m_sequences.push_back(sequence);
			}
		}
		else if (isMdxTag(chunkHeader.tag, u8"GLBS"))
		{
#ifdef DEBUG
			std::cerr << "GLBS" << std::endl;
#endif
			m_globalSequences.clear();

			while (chunkHeader.size > 0)
			{
				long32 globalSequence = 0;
				std::streamsize s = 0;
				wc3lib::read(istream, globalSequence, s);
				chunkHeader.size -= s;
				size += s;
				m_globalSequences.push_back(globalSequence);
			}
		}
		else if (isMdxTag(chunkHeader.tag, u8"MTLS"))
		{
#ifdef DEBUG
			std::cerr << "MTLS" << std::endl;
#endif
			m_materials.clear();

			while (chunkHeader.size > 0)
			{
				Material material;
				const std::streamsize s = material.read(istream, m_modelVersion);
				chunkHeader.size -= s;
				size += s;
				m_materials.push_back(material);
			}
		}
		else if (isMdxTag(chunkHeader.tag, u8"TXAN"))
		{
#ifdef DEBUG
			std::cerr << "TXAN" << std::endl;
#endif
			m_textureAnimations.clear();

			while (chunkHeader.size > 0)
			{
				TextureAnimation textureAnimation;
				const std::streamsize s = textureAnimation.read(istream);
				chunkHeader.size -= s;
				size += s;
				m_textureAnimations.push_back(textureAnimation);
			}
		}
		else if (isMdxTag(chunkHeader.tag, u8"GEOS"))
		{
#ifdef DEBUG
			std::cerr << "GEOS" << std::endl;
#endif
			m_geosets.clear();

			while (chunkHeader.size > 0)
			{
				Geoset geoset;
				const std::streamsize s = geoset.read(istream);
				chunkHeader.size -= s;
				size += s;
				m_geosets.push_back(geoset);
			}
		}
		else if (isMdxTag(chunkHeader.tag, u8"TEXS"))
		{
#ifdef DEBUG
			std::cerr << "TEXS" << std::endl;
#endif
			m_textures.clear();

			while (chunkHeader.size > 0)
			{
				Texture texture;
				const std::streamsize s = texture.read(istream);
				chunkHeader.size -= s;
				size += s;
				m_textures.push_back(texture);
			}
		}
		else if (isMdxTag(chunkHeader.tag, u8"SNDS"))
		{
#ifdef DEBUG
			std::cerr << "SNDS" << std::endl;
#endif
			// Note that this is here for completeness' sake.
			// These objects were only used at some point before Warcraft 3 released.
			m_soundTracks.clear();

			while (chunkHeader.size > 0)
			{
				SoundTrack soundTrack;
				const std::streamsize s = soundTrack.read(istream);
				chunkHeader.size -= s;
				size += s;
				m_soundTracks.push_back(soundTrack);
			}
		}
		else if (isMdxTag(chunkHeader.tag, u8"PIVT"))
		{
#ifdef DEBUG
			std::cerr << "PIVT" << std::endl;
#endif
			m_pivotPoints.clear();

			while (chunkHeader.size > 0)
			{
				VertexData pivotPoint;
				const std::streamsize s = pivotPoint.read(istream);
				chunkHeader.size -= s;
				size += s;
				m_pivotPoints.push_back(pivotPoint);
			}
		}
		else
		{
#ifdef DEBUG
			std::cerr << "Unknown tag " << chunkHeader.readableTag() << " at position " << istream.tellg() << std::endl;
#endif

			// try the next byte as start
			if (istream.tellg() < end - static_cast<std::streamsize>(chunkHeader.size))
			{
				istream.seekg(static_cast<std::streamsize>(chunkHeader.size), std::ios_base::cur);
			}
			else
			{
#ifdef DEBUG
				std::cerr << "Reached the end " << end << " at " << istream.tellg() << std::endl;
#endif

				break;
			}
		}
	}

	static std::streamsize remaing_bytes =  remainingInputStreamSize(istream);
#ifdef DEBUG
	std::cout << "Remaining bytes in MDX " << remaing_bytes << std::endl;
#endif

	return size;
}

std::streamsize Mdlx::write(OutputStream &ostream) const
{
	std::streamsize size = 0;
	writeMdxTag(ostream, u8"MDLX", size);

	// VERS
	writeMdxHeader(ostream, size, u8"VERS", sizeof(m_modelVersion));
	wc3lib::write(ostream, m_modelVersion, size);

	// MODL
	size += m_model.write(ostream);

	// SEQS
	auto p = skipMdxHeader(ostream);
	std::streamsize sequencesSize = 0;

	for (const Sequence &sequence : m_sequences)
	{
		sequencesSize += sequence.write(ostream);
	}

	auto p2 = ostream.tellp();
	ostream.seekp(p);
	writeMdxHeader(ostream, size, u8"SEQS", sequencesSize);
	ostream.seekp(p2);
	size += sequencesSize;

	// GLBS
	p = skipMdxHeader(ostream);
	std::streamsize globalSequencesSize = 0;

	for (const long32 &globalSequence : m_globalSequences)
	{
		wc3lib::write(ostream, globalSequence, globalSequencesSize);
	}

	p2 = ostream.tellp();
	ostream.seekp(p);
	writeMdxHeader(ostream, size, u8"GLBS", globalSequencesSize);
	ostream.seekp(p2);
	size += globalSequencesSize;

	// MTLS
	p = skipMdxHeader(ostream);
	std::streamsize materialsSize = 0;

	for (const Material &material : m_materials)
	{
		materialsSize += material.write(ostream, m_modelVersion);
	}

	p2 = ostream.tellp();
	ostream.seekp(p);
	writeMdxHeader(ostream, size, u8"MTLS", materialsSize);
	ostream.seekp(p2);
	size += materialsSize;

	// TXAN
	p = skipMdxHeader(ostream);
	std::streamsize textureAnimationsSize = 0;

	for (const TextureAnimation &textureAnimation : m_textureAnimations)
	{
		textureAnimationsSize += textureAnimation.write(ostream);
	}

	p2 = ostream.tellp();
	ostream.seekp(p);
	writeMdxHeader(ostream, size, u8"TXAN", textureAnimationsSize);
	ostream.seekp(p2);
	size += textureAnimationsSize;

	// GEOS
	p = skipMdxHeader(ostream);
	std::streamsize geosetsSize = 0;

	for (const Geoset &geoset : m_geosets)
	{
		geosetsSize += geoset.write(ostream);
	}

	p2 = ostream.tellp();
	ostream.seekp(p);
	writeMdxHeader(ostream, size, u8"GEOS", geosetsSize);
	ostream.seekp(p2);
	size += geosetsSize;

	// TEXS
	p = skipMdxHeader(ostream);
	std::streamsize texturesSize = 0;

	for (const Texture &texture : m_textures)
	{
		texturesSize += texture.write(ostream);
	}

	p2 = ostream.tellp();
	ostream.seekp(p);
	writeMdxHeader(ostream, size, u8"TEXS", texturesSize);
	ostream.seekp(p2);
	size += texturesSize;

	// SNDS
	p = skipMdxHeader(ostream);
	std::streamsize soundTracksSize = 0;

	for (const SoundTrack &soundTrack : m_soundTracks)
	{
		soundTracksSize += soundTrack.write(ostream);
	}

	p2 = ostream.tellp();
	ostream.seekp(p);
	writeMdxHeader(ostream, size, u8"SNDS", soundTracksSize);
	ostream.seekp(p2);
	size += soundTracksSize;

	// PIVT
	p = skipMdxHeader(ostream);
	std::streamsize pivotPointsSize = 0;

	for (const VertexData &pivotPoint : m_pivotPoints)
	{
		pivotPointsSize += pivotPoint.write(ostream);
	}

	p2 = ostream.tellp();
	ostream.seekp(p);
	writeMdxHeader(ostream, size, u8"PIVT", pivotPointsSize);
	ostream.seekp(p2);
	size += pivotPointsSize;

	return size;
}

}
