#pragma once

#include <vxLib/Stream.h>

namespace vx
{
	struct ReflectionData;

	class ReflectionManager
	{
		struct Data;

		static void initData();

		static Data* s_data;
		static bool s_initialized;

		static void serializeImpl(OutStream* outStream, const u8* src, const vx::ReflectionData* reflection);

		static bool deserializeImpl(InStream* inStream, u8* dst, const vx::ReflectionData* reflection);

	public:
		static void addData(const ReflectionData* p, size_t size);

		static const ReflectionData* find(u32 key);

		template<typename T>
		static const ReflectionData* find()
		{
			return find(T::s_reflectionId);
		}

		template<typename T>
		static void serialize(OutStream* outStream, const T* src)
		{
			serializeImpl(outStream, reinterpret_cast<const u8*>(src), find<T>());
		}

		template<typename T>
		static bool deserialize(InStream* inStream, T* dst)
		{
			return deserializeImpl(inStream, reinterpret_cast<u8*>(dst), find<T>());
		}
	};
}