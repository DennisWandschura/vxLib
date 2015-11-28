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

#include <vxLib/Allocator/StackAllocator.h>

namespace vx
{
	template<typename Super, size_t BLOCK_SIZE, size_t ALIGNMENT>
	class BitmapBlock
	{
		static_assert(GetAlignedSize<BLOCK_SIZE, ALIGNMENT>::size == BLOCK_SIZE, "");

		typedef BitmapBlock<Super, BLOCK_SIZE, ALIGNMENT> MyBitmapBlock;

		enum : size_t 
		{
			BitsSizeT = sizeof(size_t) * 8,
			ClassDataSize = sizeof(u8*) * 2 + sizeof(size_t) * 2,
			ClassAlignment = __alignof(Super),
			ClassAlignedDataSize = GetAlignedSize<ClassDataSize, ClassAlignment>::size,
			PaddingSize = ClassAlignedDataSize - ClassDataSize
		};

		u8* m_firstBlock;
		union
		{
			u8* m_bitsPtr;
			size_t m_bits;
		};
		size_t m_remainingBlocks;
		size_t m_blockCount;
		Padding<PaddingSize> m_padding;
		Super m_parent;

		u8* getBitsPtr()
		{
			return (m_blockCount <= BitsSizeT) ? (u8*)&m_bits : m_bitsPtr;
		}

		size_t countBlocks()
		{
			size_t blockCount = 0;
			auto block = m_parent.allocate(BLOCK_SIZE, ALIGNMENT);
			while (block.ptr != nullptr)
			{
				++blockCount;

				block = m_parent.allocate(BLOCK_SIZE, ALIGNMENT);
			}

			m_parent.deallocateAll();

			return blockCount;
		}

		bool findEmptyBit(u8* bitPtr, size_t* resultBit)
		{
			auto byte = 0;

			size_t block = 0;
			auto remainingBlocks = m_blockCount;
			while (remainingBlocks > 0)
			{
				auto p = bitPtr[byte];
				auto bitsToCheck = (remainingBlocks < 8) ? remainingBlocks : 8;

				for (size_t bit = 0; bit < bitsToCheck; ++bit)
				{
					auto mask = 1 << bit;
					auto tmp = (p & mask);
					if (tmp == 0)
					{
						*resultBit = block;
						return true;
					}

					++block;
				}

				remainingBlocks -= bitsToCheck;
			}

			return false;
		}

		void setBit(u8* bitPtr, size_t blockIndex)
		{
			auto byte = blockIndex / 8;
			auto bit = blockIndex & 7;

			bitPtr[byte] |= (1 << bit);
		}

		void clearBit(size_t blockIndex)
		{
			auto byte = blockIndex / 8;
			auto bit = blockIndex & 7;

			auto bitPtr = getBitsPtr();
			bitPtr[byte] &= ~(1 << bit);
		}

		void initializeImpl()
		{
			auto blockCount = countBlocks();
			if (blockCount == 0)
				return;

			auto requiredBytes = (blockCount + 7 / 8);
			if (requiredBytes <= sizeof(size_t))
			{
				// small string optimization
				m_bits = 0;
			}
			else
			{
				auto requiredBlocksForBits = (requiredBytes + BLOCK_SIZE - 1) / BLOCK_SIZE;

				if (blockCount <= requiredBlocksForBits)
						return;

				blockCount -= requiredBlocksForBits;

				auto bitBlock = m_parent.allocate(BLOCK_SIZE * requiredBlocksForBits, ALIGNMENT);
				m_bitsPtr = bitBlock.ptr;
				memset(m_bitsPtr, 0, bitBlock.size);
			}

			m_firstBlock = m_parent.allocate(BLOCK_SIZE * blockCount, ALIGNMENT).ptr;

			m_remainingBlocks = blockCount;
			m_blockCount = blockCount;
		}

	public:
		BitmapBlock() : m_firstBlock(nullptr), m_bitsPtr(nullptr), m_remainingBlocks(0), m_blockCount(0), m_parent() { initializeImpl(); }

		explicit BitmapBlock(const AllocatedBlock &block) : m_firstBlock(nullptr), m_bitsPtr(nullptr), m_remainingBlocks(0), m_blockCount(0), m_parent(block) { initializeImpl(); }

		~BitmapBlock() {}

		void initialize(const AllocatedBlock &block)
		{
			m_parent.initialize(block);

			initializeImpl();
		}

		AllocatedBlock release()
		{
			m_firstBlock = m_bitsPtr = nullptr;
			m_remainingBlocks = m_blockCount = 0;
			return m_parent.release();
		}

		AllocatedBlock allocate(size_t size, size_t alignment)
		{
			if (size > BLOCK_SIZE || alignment > ALIGNMENT || m_remainingBlocks == 0)
			{
				return{ nullptr, 0 };
			}

			size_t blockIndex = 0;

			auto bitPtr = getBitsPtr();
			if (!findEmptyBit(bitPtr, &blockIndex))
			{
				return{ nullptr, 0 };
			}

			setBit(bitPtr, blockIndex);
			auto offset = blockIndex * BLOCK_SIZE;
			--m_remainingBlocks;

			return{ m_firstBlock + offset, BLOCK_SIZE };
		}

		u32 deallocate(const AllocatedBlock &block)
		{
			if (block.size != BLOCK_SIZE)
			{
				return 0;
			}

			auto blockIndex = (block.ptr - m_firstBlock) / BLOCK_SIZE;
			clearBit(blockIndex);

			++m_remainingBlocks;

			return 1;
		}

		void deallocateAll()
		{
			m_parent.deallocateAll();
		}

		bool contains(const AllocatedBlock &block) const
		{
			return (block.size == BLOCK_SIZE) && m_parent.contains(block);
		}

		void print() const
		{
			m_parent.print();
			printf("blocksize: %llu, alignment: %llu\n", BLOCK_SIZE, ALIGNMENT);
		}

		static void printStatic()
		{
			printf("blocksize: %llu, alignment: %llu\n", BLOCK_SIZE, ALIGNMENT);
		}
	};
}