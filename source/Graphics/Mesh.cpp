#include <vxLib\Graphics\Mesh.h>
#include <vxLib\util\streamHelper.h>
#include <fstream>

namespace vx
{
	Mesh::Mesh(U8 *pMemory, U32 vertexCount, U32 indexCount)
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

	/*void Mesh::allocateMemory()
	{
		auto sizeVertexFloat2 = sizeof(vx::float2) * m_vertexCount;
		auto sizeVertexFloat3 = sizeof(vx::float3) * m_vertexCount;
		m_pMemory = (U8*)::operator new(sizeVertexFloat3 * 4 + sizeVertexFloat2 + sizeof(U32) * m_indexCount);
	}*/

	void Mesh::setPointers(U8* p)
	{
		//auto memPtr = m_pMemory;

		auto sizeF3 = sizeof(MeshVertex) * m_vertexCount;
		m_pVertices = (MeshVertex*)p;

		p += sizeF3;

		m_pIndices = (U32*)p;
	}

	/*bool Mesh::load(std::istream &inStream)
	{
		assert(m_pMemory == nullptr);
		inStream.flags(std::istream::binary);

		if (!read(inStream, m_vertexCount))
			return false;
		if (!read(inStream, m_indexCount))
			return false;

		allocateMemory();

		auto totalSize = (sizeof(vx::float3) * 4 + sizeof(vx::float2)) * m_vertexCount + sizeof(U32) * m_indexCount;
		if (!read(inStream, m_pMemory, totalSize))
			return false;

		setPointers();

		return true;
	}*/

	void Mesh::load(const U8 *src, U8* pMemory)
	{
		m_vertexCount = *reinterpret_cast<const U32*>(src);
		src += sizeof(U32);

		m_indexCount = *reinterpret_cast<const U32*>(src);
		src += sizeof(U32);

		// allocate space
		//allocateMemory();
	//	m_pMemory = pMemory;
		auto totalSize = sizeof(MeshVertex) * m_vertexCount + sizeof(U32) * m_indexCount;
		memcpy(pMemory, src, totalSize);

		setPointers(pMemory);
	}

	void Mesh::save(U8 **pPtr) const
	{
		U8 *ptr = *pPtr;

		*reinterpret_cast<U32*>(ptr) = m_vertexCount;
		ptr += sizeof(U32);

		*reinterpret_cast<U32*>(ptr) = m_indexCount;
		ptr += sizeof(U32);

		auto totalSize = sizeof(MeshVertex) * m_vertexCount + sizeof(U32) * m_indexCount;
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

		//auto totalSize = sizeof(MeshVertex) * m_vertexCount + sizeof(U32) * m_indexCount;
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

	/*bool Mesh::loadFromFile(const char *filename)
	{
		std::ifstream infile(filename, std::ifstream::in | std::ifstream::binary);

		if (!infile.is_open())
			return false;

		return load(infile);
	}*/
}