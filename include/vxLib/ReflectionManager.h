#pragma once

#include <vxLib/types.h>

namespace vx
{
	struct ReflectionData;

	class ReflectionManager
	{
		struct Data;

		static void initData();

		static Data* s_data;
		static bool s_initialized;

	public:
		static void addData(const ReflectionData* p, uint64_t size);

		static const ReflectionData* find(uint32_t key);
	};
}