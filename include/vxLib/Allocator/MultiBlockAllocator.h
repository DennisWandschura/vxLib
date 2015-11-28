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
	/*
	BLOCK_SIZE: size of allocation, needs to be aligned by ALIGNMENT
	ALIGNMENT: alignment of blocks
	MAX_BLOCK_COUNT: maximum amount of blocks used by an allocation
	*/
	template<size_t BLOCK_SIZE, size_t ALIGNMENT, size_t MAX_BLOCK_COUNT>
	class MultiBlockAllocator
	{
		static_assert(GetAlignedSize<BLOCK_SIZE, ALIGNMENT>::size == BLOCK_SIZE, "");

		enum :size_t {BitsSizeT = sizeof(size_t) * 8};

		u8* m_firstBlock;
		union
		{
			u8* m_bitsPtr;
			size_t m_bits;
		};
		size_t m_remainingBlocks;
		size_t m_blockCount;
		vx::AllocatedBlock m_block;

		u8* getBitPtr()
		{
			return (m_blockCount <= BitsSizeT) ? (u8*)&m_bits : m_bitsPtr;
		}

		size_t countBlocks(const AllocatedBlock &block)
		{
			auto alignedPtr = getAlignedPtr(block.ptr, ALIGNMENT);
			auto offset = (size_t)(alignedPtr - block.ptr);
			auto remainingSize = block.size - offset;

			return remainingSize / BLOCK_SIZE;
		}

		bool findEmptyBit(u8* bitsPtr, size_t* resultBit, size_t blockCount)
		{
			auto byte = 0;

			size_t freeConiguousBlocks = 0;

			size_t resultBlock = 0;
			size_t block = 0;
			auto remainingBlocks = m_blockCount;
			while (remainingBlocks >= blockCount)
			{
				auto p = bitsPtr[byte];
				auto bitsToCheck = (remainingBlocks < 8) ? remainingBlocks : 8;
				if (bitsToCheck < blockCount)
					break;

				for (size_t bit = 0; bit < bitsToCheck; ++bit)
				{
					auto mask = 1 << bit;
					auto tmp = (p & mask);
					if (tmp == 0)
					{
						++freeConiguousBlocks;

						if (freeConiguousBlocks >= blockCount)
						{
							*resultBit = resultBlock;
							return true;
						}
					}
					else
					{
						resultBlock = block + 1;
						freeConiguousBlocks = 0;
					}

					++block;
				}

				remainingBlocks -= bitsToCheck;
			}

			return false;
		}

		void setBits(u8* bitsPtr, size_t blockIndex, size_t blockCount)
		{
			for (size_t i = 0; i < blockCount; ++i)
			{
				auto blockIdx = blockIndex + i;

				auto byte = blockIdx / 8;
				auto bit = blockIdx & 7;

				bitsPtr[byte] |= (1 << bit);
			}
		}

		void clearBit(size_t blockIndex, size_t blockCount)
		{
			auto bitsPtr = getBitPtr();

			for (size_t i = 0; i < blockCount; ++i)
			{
				auto blockIdx = blockIndex + i;

				auto byte = blockIdx / 8;
				auto bit = blockIdx & 7;

				bitsPtr[byte] &= ~(1 << bit);
			}
		}

	public:
		MultiBlockAllocator() : m_firstBlock(nullptr), m_bitsPtr(nullptr), m_remainingBlocks(0), m_blockCount(0), m_block() { }

		explicit MultiBlockAllocator(const AllocatedBlock &block) : m_firstBlock(nullptr), m_bitsPtr(nullptr), m_remainingBlocks(0), m_blockCount(0), m_block(block){ initialize(block); }

		~MultiBlockAllocator() {}

		void initialize(const AllocatedBlock &block)
		{
			auto blockCount = countBlocks(block);
			if (blockCount == 0)
				return;

			if (blockCount <= BitsSizeT)
			{
				m_bits = 0;
				m_firstBlock = getAlignedPtr(block.ptr, ALIGNMENT);
			}
			else
			{
				auto requiredBytes = (blockCount + 7 / 8);
				auto requiredBlocksForBits = (requiredBytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
				if (requiredBlocksForBits >= blockCount)
					return;

				blockCount -= requiredBlocksForBits;

				auto bitblockSize = requiredBlocksForBits * BLOCK_SIZE;
				m_bitsPtr = getAlignedPtr(block.ptr, ALIGNMENT);
				memset(m_bitsPtr, 0, bitblockSize);

				m_firstBlock = m_bitsPtr + bitblockSize;
			}

			m_remainingBlocks = blockCount;
			m_blockCount = blockCount;
			m_block = block;
		}

		AllocatedBlock release()
		{
			m_firstBlock = m_bitsPtr = nullptr;
			m_remainingBlocks = 0;
			
			auto blck = m_block;
			m_block.ptr = nullptr;
			m_block.size = 0;

			return blck;
		}

		AllocatedBlock allocate(size_t size, size_t alignment)
		{
			if(alignment > ALIGNMENT)
				return{ nullptr, 0 };

			auto alignedSize = getAlignedSize(size, alignment);

			auto blockCount = (alignedSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
			if(blockCount > m_remainingBlocks || blockCount > MAX_BLOCK_COUNT)
				return{ nullptr, 0 };

			auto bitsPtr = getBitPtr();
			size_t resultBlock = 0;
			if(!findEmptyBit(bitsPtr,&resultBlock, blockCount))
				return{ nullptr, 0 };

			setBits(bitsPtr, resultBlock, blockCount);
			m_remainingBlocks -= blockCount;

			auto offset = BLOCK_SIZE * resultBlock;

			return{ m_firstBlock + offset, alignedSize };
		}

		u32 deallocate(const AllocatedBlock &block)
		{
			VX_ASSERT(block.ptr >= m_firstBlock);

			auto blockIndex = (block.ptr - m_firstBlock) / BLOCK_SIZE;
			VX_ASSERT(blockIndex < m_blockCount);

			auto blockCount = (block.size + BLOCK_SIZE - 1) / BLOCK_SIZE;
			clearBit(blockIndex, blockCount);

			m_remainingBlocks += blockCount;;

			return 1;
		}

		void deallocateAll()
		{
		}

		bool contains(const AllocatedBlock &block) const
		{
			auto last = m_firstBlock + BLOCK_SIZE * m_blockCount;
			return (block.ptr >= m_firstBlock) && (block.ptr < last);
		}

		void print() const
		{
			printStatic();
		}

		static void printStatic()
		{
			printf("blocksize: %llu, alignment: %llu\n", BLOCK_SIZE, ALIGNMENT);
		}
	};
}