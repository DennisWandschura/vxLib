/*
The MIT License(MIT)

Copyright(c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vxLib/Graphics/Mesh.h>
#include <vxLib/File/File.h>
#include <algorithm>

namespace vx
{
	namespace Graphics
	{
		Mesh::Mesh()
			:m_pVertices(nullptr),
			m_pIndices(nullptr),
			m_vertexCount(0),
			m_indexCount(0)
		{
		}

		Mesh::Mesh(u8 *pMemory, u32 vertexCount, u32 indexCount)
			:m_pVertices(nullptr),
			m_pIndices(nullptr),
			m_vertexCount(vertexCount),
			m_indexCount(indexCount)
		{
			setPointers(pMemory);
		}

		Mesh::Mesh(Mesh &&rhs) noexcept
			:m_pVertices(rhs.m_pVertices),
			m_pIndices(rhs.m_pIndices),
			m_vertexCount(rhs.m_vertexCount),
			m_indexCount(rhs.m_indexCount)
		{
		}

		Mesh::~Mesh()
		{
		}

		Mesh& Mesh::operator = (Mesh &&rhs) noexcept
		{
			if (this != &rhs)
			{
				this->swap(rhs);

			}
			return *this;
		}

		void Mesh::swap(Mesh &rhs) noexcept
		{
			std::swap(m_pVertices, rhs.m_pVertices);
			std::swap(m_pIndices, rhs.m_pIndices);
			std::swap(m_vertexCount, rhs.m_vertexCount);
			std::swap(m_indexCount, rhs.m_indexCount);
		}

		void Mesh::setPointers(u8* p)
		{
			auto verticesSizeInBytes = sizeof(MeshVertex) * m_vertexCount;
			m_pVertices = (MeshVertex*)p;

			p += verticesSizeInBytes;

			m_pIndices = (u32*)p;
		}

		const u8* Mesh::loadFromMemoryDataSize(const u8 *src, u64* dataSizeInBytes)
		{
			m_vertexCount = *reinterpret_cast<const u32*>(src);
			src += sizeof(u32);

			m_indexCount = *reinterpret_cast<const u32*>(src);
			src += sizeof(u32);

			auto totalSize = Mesh::getArraySize(m_vertexCount, m_indexCount);

			*dataSizeInBytes = totalSize;

			return src;
		}

		const u8* Mesh::loadFromMemoryData(const u8* src, u8* dst, u64 size)
		{
			memcpy(dst, src, size);
			src += size;

			setPointers(dst);

			return src;
		}

		u8* Mesh::saveToMemory(u8 *ptr) const
		{
			*reinterpret_cast<u32*>(ptr) = m_vertexCount;
			ptr += sizeof(u32);

			*reinterpret_cast<u32*>(ptr) = m_indexCount;
			ptr += sizeof(u32);

			auto totalSize = Mesh::getArraySize(m_vertexCount, m_indexCount);
			memcpy(ptr, (u8*)m_pVertices, totalSize);

			return (ptr + totalSize);
		}

		bool Mesh::saveToFile(File* file) const
		{
			if (!file->write(m_vertexCount))
			{
				puts("Mesh::saveToFile(): Error writing to file");
				return false;
			}

			if (!file->write(m_indexCount))
			{
				puts("Mesh::saveToFile(): Error writing to file");
				return false;
			}

			auto totalSize = sizeof(MeshVertex) * m_vertexCount + sizeof(u32) * m_indexCount;
			if (!file->write((u8*)m_pVertices, totalSize))
			{
				puts("Mesh::saveToFile(): Error writing to file");
				return false;
			}

			return true;
		}

		u64 Mesh::getArraySize(u32 vertexCount, u32 indexCount)
		{
			return sizeof(MeshVertex) * vertexCount + sizeof(u32) * indexCount;
		}
	}
}