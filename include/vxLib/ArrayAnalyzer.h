#pragma once

#include <vxLib/Container/StringArray.h>
#include <atomic>
#include <algorithm>

namespace vx
{
	struct ArrayStats
	{
		u32 m_firstReserveCount;
		u32 m_reserveLessCount;
		u32 m_reserveCount;
		u32 m_maxCapacity;
		u32 m_maxSize;
		u32 m_copyCount;
		u32 m_swapCount;
		u32 m_valueSize;
		u32 m_size;
		u32 m_capacity;
		const char* m_arrayType;
		const char* m_valueTypeName;
		void* m_returnAddress;

	public:
		ArrayStats()
			:m_firstReserveCount(0),
			m_maxCapacity(0),
			m_reserveCount(0),
			m_maxSize(0),
			m_copyCount(0),
			m_reserveLessCount(0),
			m_swapCount(0),
			m_valueSize(0),
			m_size(0),
			m_capacity(0),
			m_arrayType(nullptr),
			m_valueTypeName(nullptr),
			m_returnAddress(nullptr)
		{

		}

		void init(void* returnAddress, const char* arrayType, u32 valueSize, const char* valueTypeName)
		{
			m_firstReserveCount = 0;
			m_maxCapacity = 0;
			m_reserveCount = 0;
			m_maxSize = 0;
			m_copyCount = 0;
			m_reserveLessCount = 0;
			m_swapCount = 0;
			m_arrayType = arrayType;
			m_valueSize = valueSize;
			m_valueTypeName = valueTypeName;
			m_returnAddress = returnAddress;
			m_size = 0;
			m_capacity = 0;

			VX_ASSERT(m_arrayType);
			VX_ASSERT(m_valueTypeName);
			VX_ASSERT(m_returnAddress);
		}

		void updateMaxCapacity(u32 c)
		{
			m_maxCapacity = std::max(c, m_maxCapacity);
		}

		void updateMaxSize(u32 c)
		{
			m_maxSize = std::max(c, m_maxSize);
		}

		void swap(ArrayStats &rhs)
		{
			std::swap(m_firstReserveCount, rhs.m_firstReserveCount);
			std::swap(m_maxCapacity, rhs.m_maxCapacity);
			std::swap(m_reserveCount, rhs.m_reserveCount);
			std::swap(m_maxSize, rhs.m_maxSize);
			std::swap(m_copyCount, rhs.m_copyCount);
			std::swap(m_reserveLessCount, rhs.m_reserveLessCount);
			std::swap(m_swapCount, rhs.m_swapCount);
			std::swap(m_arrayType, rhs.m_arrayType);
			std::swap(m_valueSize, rhs.m_valueSize);
			std::swap(m_size, rhs.m_size);
			std::swap(m_capacity, rhs.m_capacity);
			std::swap(m_valueTypeName, rhs.m_valueTypeName);
			std::swap(m_returnAddress, rhs.m_returnAddress);
		}
	};

	struct ArrayStatsExport
	{
		u32 m_firstReserveCount;
		u32 m_maxCapacity;
		u32 m_reserveCount;
		u32 m_maxSize;
		u32 m_copyCount;
		u32 m_reserveLessCount;
		u32 m_swapCount;
		u32 m_valueSize;
		u32 m_lineNumber;
		u32 m_size;
		u32 m_capacity;
		vx::StringArray<96> m_arrayType;
		vx::StringArray<96> m_valueTypeName;
		vx::StringArray<128> m_returnAddress;
		vx::StringArray<256> m_fileName;

	public:
		ArrayStatsExport()
			:m_firstReserveCount(0),
			m_maxCapacity(0),
			m_reserveCount(0),
			m_maxSize(0),
			m_copyCount(0),
			m_reserveLessCount(0),
			m_swapCount(0),
			m_arrayType(),
			m_valueSize(0),
			m_lineNumber(0),
			m_size(0),
			m_capacity(0),
			m_valueTypeName(),
			m_returnAddress(),
			m_fileName()
		{
		}

		void assign(const ArrayStats &rhs, const StringArray<128> &returnAddress, u32 lineNumber, const vx::StringArray<256> &fileName)
		{
			m_firstReserveCount = rhs.m_firstReserveCount;
			m_maxCapacity = rhs.m_maxCapacity;
			m_reserveCount = rhs.m_reserveCount;
			m_maxSize = rhs.m_maxSize;
			m_copyCount = rhs.m_copyCount;
			m_reserveLessCount = rhs.m_reserveLessCount;
			m_swapCount = rhs.m_swapCount;
			m_arrayType = rhs.m_arrayType;
			m_valueSize = rhs.m_valueSize;
			m_size = rhs.m_size;
			m_capacity = rhs.m_capacity;
			m_lineNumber = lineNumber;
			m_valueTypeName = rhs.m_valueTypeName;
			m_returnAddress = returnAddress;
			m_fileName = fileName;
		}
	};

	class ArrayAnalyzer
	{
		static const u32 CAPACITY{ (500 MBYTE) / sizeof(ArrayStats) };
		static std::atomic_uint64_t s_counter;
		static ArrayStats* s_data;

	public:
		static void initialize()
		{
			if(s_data == nullptr)
				s_data = (ArrayStats*)_aligned_malloc(sizeof(ArrayStats) * CAPACITY, __alignof(ArrayStats));
		}

		static void shutdown()
		{
			if (s_data)
			{
				_aligned_free(s_data);
				s_data = nullptr;
			}
		}

		static const ArrayStats* getData() { return s_data; }

		static u64 getDataCount() { return s_counter.load(); }

		static void addData(const ArrayStats &stats)
		{
			auto idx = s_counter.fetch_add(1);
			VX_ASSERT(idx < CAPACITY);
			s_data[idx] = stats;

			VX_ASSERT(s_data[idx].m_arrayType);
			VX_ASSERT(s_data[idx].m_valueTypeName);
			VX_ASSERT(s_data[idx].m_returnAddress);
		}
	};
}

#ifdef _VX_ARRAY_ANALYZER

#define VX_REGISTER_ANALYZER \
getArrayStats().init(_ReturnAddress(), type_info(*this).getName(), sizeof(value_type), type_info<value_type>().getName())
#else
#define VX_REGISTER_ANALYZER
#endif