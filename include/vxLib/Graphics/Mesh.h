#ifndef __VX_MESH_H
#define __VX_MESH_H
#pragma once

#include <vxLib/math/Vector.h>
#include <ostream>

namespace vx
{
	struct MeshVertex
	{
		vx::float3 position;
		vx::float2 texCoords;
		vx::float3 normal;
		vx::float3 tangent;
		vx::float3 bitangent;
	};

	class Mesh
	{
		MeshVertex* m_pVertices{ nullptr };
		U32* m_pIndices{ nullptr };
		U32 m_vertexCount{ 0 };
		U32 m_indexCount{ 0 };

		//void allocateMemory();
		void setPointers(U8* p);

	public:
		Mesh(){}
		// takes a pointer to memory filled with vertices and indices
		Mesh(U8 *pMemory, U32 vertexCount, U32 indexCount);
		Mesh(const Mesh&) = delete;
		Mesh(Mesh &&rhs) noexcept;
		~Mesh();

		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh &&rhs) noexcept;

		void swap(Mesh &rhs) noexcept;

		//bool load(std::istream &is);
		bool save(std::ostream &os) const;
		/* 
		takes a pointer to binary source data and a pointer to store the data
		*/
		void load(const U8 *src, U8* pMemory);
		void save(U8 **ptr) const;
		//bool loadFromFile(const char *filename);
		bool saveToFile(const char *filename) const;

		const MeshVertex* getVertices() const { return m_pVertices; }
		const U32* getIndices() const { return m_pIndices; }
		U32 getVertexCount() const { return m_vertexCount; }
		U32 getIndexCount() const { return m_indexCount; }
	};
}
#endif