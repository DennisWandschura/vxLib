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