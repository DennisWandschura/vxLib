/*
The MIT License (MIT)

Copyright (c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <vxlib/Graphics/FontAtlas.h>
#include <algorithm>

namespace vx
{
	namespace Graphics
	{
		inline const char* getCharacter(const char *str, char in)
		{
			char c = *str;
			while (c != in)
			{
				c = *str;
				++str;
			}

			return str;
		}

		FontAtlasEntry::FontAtlasEntry()
			:code(0),
			x(0),
			y(0),
			width(0),
			height(0),
			offsetX(0),
			offsetY(0),
			advanceX(0)
		{
		}

		FontAtlas::FontAtlas()
			:m_data(),
			m_end(nullptr),
			m_allocatedSize(0)
		{
		}

		FontAtlas::FontAtlas(FontAtlas &&rhs)
			: m_data(rhs.m_data),
			m_end(rhs.m_end),
			m_allocatedSize(rhs.m_allocatedSize)
		{
			rhs.m_data = nullptr;
			rhs.m_allocatedSize = 0;
		}

		FontAtlas::~FontAtlas()
		{
		}

		FontAtlas& FontAtlas::operator = (FontAtlas &&rhs)
		{
			if (this != &rhs)
			{
				std::swap(m_data, rhs.m_data);
				std::swap(m_end, rhs.m_end);
				std::swap(m_allocatedSize, rhs.m_allocatedSize);
			}
			return *this;
		}

		vx::AllocatedBlock FontAtlas::release()
		{
			vx::AllocatedBlock block = {(u8*)m_data, m_allocatedSize};

			m_data = nullptr;
			m_end = nullptr;
			m_allocatedSize = 0;

			return block;
		}

		FontAtlasEntry FontAtlas::readEntry(std::ifstream &infile)
		{
			FontAtlasEntry entry;
			char input;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}
			infile >> entry.code;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}
			infile >> entry.x;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}
			infile >> entry.y;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}
			infile >> entry.width;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}
			infile >> entry.height;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}
			infile >> entry.offsetX;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}
			infile >> entry.offsetY;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}
			infile >> entry.advanceX;

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}

			return entry;
		}

		size_t FontAtlas::readEntry(const char *ptr, FontAtlasEntry &entry)
		{
			auto p = ptr;

			p = getCharacter(p, '=');
			sscanf(p, "%u", &entry.code);

			p = getCharacter(p, '=');
			sscanf(p, "%f", &entry.x);

			p = getCharacter(p, '=');
			sscanf(p, "%f", &entry.y);

			p = getCharacter(p, '=');
			sscanf(p, "%f", &entry.width);

			p = getCharacter(p, '=');
			sscanf(p, "%f", &entry.height);

			p = getCharacter(p, '=');
			sscanf(p, "%f", &entry.offsetX);

			p = getCharacter(p, '=');
			sscanf(p, "%f", &entry.offsetY);

			p = getCharacter(p, '=');
			sscanf(p, "%f", &entry.advanceX);

			p = getCharacter(p, '=');

			p = getCharacter(p, '=');

			size_t result = p - ptr;

			return result;
		}

		bool FontAtlas::loadFromFile(const char *file, AllocationCallbackSignature allocFn)
		{
			std::ifstream infile;

			infile.open(file);
			if (!infile.is_open())
				return false;

			char input;
			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}

			infile.get(input);
			while (input != '=')
			{
				infile.get(input);
			}

			u32 entryCount = 0;
			infile >> entryCount;

			VX_ASSERT(entryCount != 0);

			auto allocatedBlock = allocFn(sizeof(FontAtlasEntry) * entryCount, __alignof(FontAtlasEntry));
			if (allocatedBlock.ptr == nullptr)
				return false;

			m_data = (FontAtlasEntry*)allocatedBlock.ptr;
			m_allocatedSize = allocatedBlock.size;

			for (u32 i = 0; i < entryCount; ++i)
			{
				auto entry = readEntry(infile);

				m_data[i] = entry;
			}

			m_end = m_data + entryCount;

			std::sort(m_data, m_end, [](const FontAtlasEntry &l, const FontAtlasEntry &r)
			{
				return l.code < r.code;
			});

			return true;
		}

		bool FontAtlas::loadFromMemory(const char *data, AllocationCallbackSignature allocFn)
		{
			if (data == nullptr)
				return false;

			auto ptr = data;

			char input = *ptr;
			while (input != '=')
			{
				input = *ptr;
				++ptr;
			}

			input = *ptr;
			while (input != '=')
			{
				input = *ptr;
				++ptr;
			}

			u32 entryCount = 0;
			sscanf(ptr, "%u", &entryCount);

			VX_ASSERT(entryCount != 0);

			//ptr += r;
			ptr += sizeof(u32);

			auto allocatedBlock = allocFn(sizeof(FontAtlasEntry) * entryCount, __alignof(FontAtlasEntry));
			if (allocatedBlock.ptr == nullptr)
				return false;

			m_data = (FontAtlasEntry*)allocatedBlock.ptr;
			m_allocatedSize = allocatedBlock.size;

			for (u32 i = 0; i < entryCount; ++i)
			{
				FontAtlasEntry entry;
				auto n = readEntry(ptr, entry);

				ptr += n;

				m_data[i] = entry;
			}

			m_end = m_data + entryCount;

			std::sort(m_data, m_end, [](const FontAtlasEntry &l, const FontAtlasEntry &r)
			{
				return l.code < r.code;
			});

			return true;
		}

		const FontAtlasEntry* FontAtlas::getEntry(u32 code) const
		{
			const FontAtlasEntry *pEntry = nullptr;

			const auto cmp = [](const FontAtlasEntry &entry, u32 key)
			{
				return entry.code < key;
			};

			auto it = std::lower_bound(m_data, m_end, code, cmp);

			if (it != m_end && (it->code == code))
			{
				pEntry = &*it;
			}

			return pEntry;
		}
	}
}