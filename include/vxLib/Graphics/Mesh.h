#pragma once
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

#include <vxLib/math/Vector.h>

namespace vx
{
	class File;
	class Allocator;

	struct MeshVertex
	{
		float3 position;
		float2 texCoords;
		float3 normal;
		float3 tangent;
		float3 bitangent;
	};

	class Mesh
	{
		MeshVertex* m_pVertices;
		u32* m_pIndices;
		u32 m_vertexCount;
		u32 m_indexCount;

		//void allocateMemory();
		void setPointers(u8* p);

	public:
		Mesh();
		// takes a pointer to memory filled with vertices and indices
		Mesh(u8 *pMemory, u32 vertexCount, u32 indexCount);
		Mesh(const Mesh&) = delete;
		Mesh(Mesh &&rhs) noexcept;
		~Mesh();

		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh &&rhs) noexcept;

		void swap(Mesh &rhs) noexcept;

		const u8* loadFromMemory(const u8 *src, vx::Allocator* allocator);

		const u8* loadFromMemoryDataSize(const u8 *src, u32* dataSizeInBytes);
		const u8* loadFromMemoryData(const u8* src, u8* dst, u32 size);
		u8* saveToMemory(u8 *ptr) const;

		bool saveToFile(File* file) const;

		const MeshVertex* getVertices() const { return m_pVertices; }
		const u32* getIndices() const { return m_pIndices; }
		u32 getVertexCount() const { return m_vertexCount; }
		u32 getIndexCount() const { return m_indexCount; }

		static u64 getArraySize(u32 vertexCount, u32 indexCount);
	};
}