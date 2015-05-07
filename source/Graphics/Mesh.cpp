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

#include <vxLib\Graphics\Mesh.h>
#include <vxLib\util\streamHelper.h>
#include <fstream>

namespace vx
{
	Mesh::Mesh(u8 *pMemory, u32 vertexCount, u32 indexCount)
		:m_vertexCount(vertexCount),
		m_indexCount(indexCount)
	{
		setPointers(pMemory);
	}

	Mesh::Mesh(Mesh &&rhs)
		:m_pVertices(rhs.m_pVertices),
		m_pIndices(rhs.m_pIndices),
		m_vertexCount(rhs.m_vertexCount),
		m_indexCount(rhs.m_indexCount)
	{
	}

	Mesh::~Mesh()
	{
	}

	Mesh& Mesh::operator = (Mesh &&rhs)
	{
		if (this != &rhs)
		{	
			this->swap(rhs);

		}
		return *this;
	}

	void Mesh::swap(Mesh &rhs)
	{
		std::swap(m_pVertices, rhs.m_pVertices);
		std::swap(m_pIndices, rhs.m_pIndices);
		std::swap(m_vertexCount, rhs.m_vertexCount);
		std::swap(m_indexCount, rhs.m_indexCount);
	}

	void Mesh::setPointers(u8* p)
	{
		auto sizeF3 = sizeof(MeshVertex) * m_vertexCount;
		m_pVertices = (MeshVertex*)p;

		p += sizeF3;

		m_pIndices = (u32*)p;
	}

	void Mesh::load(const u8 *src, u8* pMemory)
	{
		m_vertexCount = *reinterpret_cast<const u32*>(src);
		src += sizeof(u32);

		m_indexCount = *reinterpret_cast<const u32*>(src);
		src += sizeof(u32);

		auto totalSize = sizeof(MeshVertex) * m_vertexCount + sizeof(u32) * m_indexCount;
		memcpy(pMemory, src, totalSize);

		setPointers(pMemory);
	}

	void Mesh::save(u8 **pPtr) const
	{
		u8 *ptr = *pPtr;

		*reinterpret_cast<u32*>(ptr) = m_vertexCount;
		ptr += sizeof(u32);

		*reinterpret_cast<u32*>(ptr) = m_indexCount;
		ptr += sizeof(u32);

		auto totalSize = sizeof(MeshVertex) * m_vertexCount + sizeof(u32) * m_indexCount;
		memcpy(ptr, m_pVertices, totalSize);

		*pPtr = (ptr + totalSize);
	}

	bool Mesh::save(std::ostream &outStream) const
	{
		outStream.flags(std::ofstream::binary);

		if (!write(outStream, m_vertexCount))
			return false;
		if (!write(outStream, m_indexCount))
			return false;

		if (!write(outStream, m_pVertices, m_vertexCount))
			return false;
		if (!write(outStream, m_pIndices, m_indexCount))
			return false;

		return true;
	}

	bool Mesh::saveToFile(const char *filename) const
	{
		std::ofstream outfile(filename, std::ofstream::out | std::ofstream::binary);

		if (!outfile.is_open())
		{
			printf("Mesh::saveToFile: could not open file '%s' !\n", filename);
			return false;
		}

		return save(outfile);
	}
}