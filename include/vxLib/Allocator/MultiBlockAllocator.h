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
#include <vxLib/util/bitops.h>

namespace vx
{
	/*
	BLOCK_SIZE: size of allocation, needs to be aligned by ALIGNMENT
	ALIGNMENT: alignment of blocks
	MAX_BLOCK_COUNT: maximum amount of blocks used by an allocation
	*/
	template<u64 BLOCK_SIZE, u64 ALIGNMENT, u64 MAX_BLOCK_COUNT>
	class MultiBlockAllocator
	{
		static_assert(GetAlignedSize<BLOCK_SIZE, ALIGNMENT>::size == BLOCK_SIZE, "");

		enum :u64 {BitsSizeT = sizeof(u64) * 8};

		u8* m_firstBlock;
		union
		{
			u32* m_bitsPtr;
			u64 m_bits;
		};
		u64 m_remainingBlocks;
		u64 m_blockCount;
		vx::AllocatedBlock m_block;

		u32* getBitPtr()
		{
			return (m_blockCount <= BitsSizeT) ? (u32*)&m_bits : m_bitsPtr;
		}

		u64 countBlocks(const AllocatedBlock &block)
		{
			auto alignedPtr = getAlignedPtr(block.ptr, ALIGNMENT);
			auto offset = (u64)(alignedPtr - block.ptr);
			auto remainingSize = block.size - offset;

			return remainingSize / BLOCK_SIZE;
		}

		bool findEmptyBit(u32* bitsPtr, u64* resultBit, u64 blockCount)
		{
			u64 freeConiguousBlocks = 0;

			u64 resultBlock = 0;
			u64 block = 0;

			auto blocksNeeded = blockCount;
			auto remainingBlocks = m_blockCount;
			while (remainingBlocks >= blockCount)
			{
				auto p = *bitsPtr++;;
				auto bitsToCheck = (remainingBlocks < 32) ? remainingBlocks : 32;

				int offset = (freeConiguousBlocks == 0) ? ntz(p) : 0;
				auto blocks = bitsToCheck - offset;
				blocks = (blocksNeeded < blocks) ? blocksNeeded : blocks;

				auto index = ffstrl(rev(p >> offset), (int)blocks);
				if (index != 32)
				{
					if (freeConiguousBlocks == 0)
					{
						resultBlock = block + offset;
					}

					freeConiguousBlocks += blocks;
					blocksNeeded -= blocks;
				}
				else
				{
					freeConiguousBlocks = 0;
					blocksNeeded = blockCount;
				}

				if (freeConiguousBlocks >= blockCount)
				{
					*resultBit = resultBlock;
					return true;
				}

				block += 32;

				remainingBlocks -= bitsToCheck;
			}

			return false;
		}

		bool hasFreeBits(u32* bitsPtr, u64 blockIndex, u64 blockCount)
		{
			auto needToCheck = blockCount;
			for (u64 i = 0; i < blockCount; ++i)
			{
				auto blockIdx = blockIndex + i;

				auto byte = blockIdx / 32;
				auto bit = blockIdx & 31;

				if (bitsPtr[byte] != 0)
				{
					--needToCheck;
				}
				else
				{
					break;
				}
			}

			return (needToCheck == 0);
		}

		void setBits(u32* bitsPtr, u64 blockIndex, u64 blockCount)
		{
			for (u64 i = 0; i < blockCount; ++i)
			{
				auto blockIdx = blockIndex + i;

				auto byte = blockIdx / 32;
				auto bit = blockIdx & 31;

				bitsPtr[byte] |= (1 << bit);
			}
		}

		void clearBits(u64 blockIndex, u64 blockCount)
		{
			auto bitsPtr = getBitPtr();

			for (u64 i = 0; i < blockCount; ++i)
			{
				auto blockIdx = blockIndex + i;

				auto byte = blockIdx / 32;
				auto bit = blockIdx & 31;

				bitsPtr[byte] &= ~(1 << bit);
			}
		}

	public:
		enum : u64 {MaxAllocSize = BLOCK_SIZE * MAX_BLOCK_COUNT};

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
				m_bits = 0xffffffffffffffff;
				m_firstBlock = getAlignedPtr(block.ptr, ALIGNMENT);
			}
			else
			{
				auto requiredBytes = (blockCount + 7) / 8;
				requiredBytes = vx::getAlignedSize(requiredBytes, __alignof(u32));
				auto requiredBlocksForBits = (requiredBytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
				if (requiredBlocksForBits >= blockCount)
					return;

				blockCount -= requiredBlocksForBits;

				auto bitblockSize = requiredBlocksForBits * BLOCK_SIZE;
				m_bitsPtr = (u32*)getAlignedPtr(block.ptr, ALIGNMENT);
				std::memset(m_bitsPtr, 0xff, bitblockSize);

				m_firstBlock = reinterpret_cast<u8*>(m_bitsPtr) + bitblockSize;
			}

			m_remainingBlocks = blockCount;
			m_blockCount = blockCount;
			m_block = block;
		}

		AllocatedBlock release()
		{
			m_firstBlock = nullptr;
			m_bitsPtr = nullptr;
			m_remainingBlocks = 0;
			
			auto blck = m_block;
			m_block.ptr = nullptr;
			m_block.size = 0;

			return blck;
		}

		AllocatedBlock allocate(u64 size, u64 alignment)
		{
			if(size == 0 || alignment > ALIGNMENT)
				return{ nullptr, 0 };

			auto alignedSize = getAlignedSize(size, alignment);

			auto blockCount = (alignedSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
			if(blockCount > m_remainingBlocks || blockCount > MAX_BLOCK_COUNT)
				return{ nullptr, 0 };

			auto bitsPtr = getBitPtr();
			u64 resultBlock = 0;
			if(!findEmptyBit(bitsPtr, &resultBlock, blockCount))
				return{ nullptr, 0 };

			clearBits(resultBlock, blockCount);
			m_remainingBlocks -= blockCount;

			auto offset = BLOCK_SIZE * resultBlock;

			return{ m_firstBlock + offset, blockCount * BLOCK_SIZE };
		}

		AllocatedBlock reallocate(const AllocatedBlock &block, u64 size, u64 alignment)
		{
			bool isAligned = (getAlignedPtr(block.ptr, alignment) == block.ptr);
			auto alignedSize = getAlignedSize(size, alignment);
			if ((block.size >= alignedSize) && isAligned)
				return block;

			if (isAligned)
			{
				auto blockIndex = (block.ptr - m_firstBlock) / BLOCK_SIZE;
				auto oldBlockCount = (block.size + BLOCK_SIZE - 1) / BLOCK_SIZE;
				auto newBlockCount = (alignedSize + BLOCK_SIZE - 1) / BLOCK_SIZE;

				auto checkIndex = blockIndex + oldBlockCount;
				auto diff = newBlockCount - oldBlockCount;
				VX_ASSERT(diff != 0);

				if (hasFreeBits(getBitPtr(), checkIndex, diff))
				{
					m_remainingBlocks -= diff;
					clearBits(checkIndex, diff);
					return{block.ptr, alignedSize };
				}
			}

			auto newBlock = allocate(size, alignment);
			if (newBlock.ptr)
			{
				memcpy(newBlock.ptr, block.ptr, block.size);
				memset(block.ptr, 0, block.size);
			}

			deallocate(block);

			return newBlock;
		}

		u32 deallocate(const AllocatedBlock &block)
		{
			if (block.ptr == nullptr || block.size == 0)
				return 1;

			VX_ASSERT(block.ptr >= m_firstBlock);

			auto blockIndex = (block.ptr - m_firstBlock) / BLOCK_SIZE;
			VX_ASSERT(blockIndex < m_blockCount);

			auto blockCount = (block.size + BLOCK_SIZE - 1) / BLOCK_SIZE;
			setBits(getBitPtr(), blockIndex, blockCount);

			m_remainingBlocks += blockCount;

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