#pragma once


#include <vxLib/ReflectionManager.h>
#include <vxLib/hash.h>

namespace vx
{

	struct ReflectionDataMember;

	struct ReflectionData
	{
		const char* typeName;
		u32 size;
		u32 alignment;
		const ReflectionDataMember* members;
		u32 memberCount;
		u32 hash;

		constexpr ReflectionData(const char* _typeName, u32 sz, u32 al, const ReflectionDataMember* p, u32 count, u32 _hash)
			:typeName(_typeName), size(sz), alignment(al), members(p), memberCount(count), hash(_hash) {}
	};

	struct ReflectionDataMember
	{
		const char* memberName;
		ReflectionData typeData;
		u32 offset;

		constexpr ReflectionDataMember() :memberName(nullptr), typeData(nullptr, 0, 0, nullptr, 0, 0), offset(0) {}
		constexpr ReflectionDataMember(const char* typeName, const char* _memberName, u32 size, u32 alignment, u32 _offset, u32 hash) : memberName(_memberName), typeData(typeName, size, alignment, nullptr, 0, hash), offset(_offset) {}
	};

	template<size_t MEMBERCOUNT>
	struct ReflectionDataImpl : ReflectionData
	{
		ReflectionDataMember membersBuffer[MEMBERCOUNT];

		template<typename ...Args>
		constexpr ReflectionDataImpl(const char* typeName, u32 size, u32 alignment, u32 hash, Args&& ...args) :membersBuffer{ args... }, ReflectionData(typeName, size, alignment, membersBuffer, MEMBERCOUNT, hash)
		{
			ReflectionManager::addData(this, sizeof(ReflectionDataImpl<MEMBERCOUNT>));
		}

		constexpr ReflectionDataImpl(const char* typeName, u32 size, u32 alignment, u32 hash) : membersBuffer(), ReflectionData(typeName, size, alignment, membersBuffer, MEMBERCOUNT, hash)
		{
			ReflectionManager::addData(this, sizeof(ReflectionDataImpl<MEMBERCOUNT>));
		}
	};

	template<>
	struct ReflectionDataImpl<0> : ReflectionData
	{
		constexpr ReflectionDataImpl(const char* name, u32 size, u32 alignment, u32 hash) : ReflectionData(name, size, alignment, nullptr, 0, hash) {}
	};

	namespace detail
	{
		template<typename ...Args>
		constexpr auto createReflectionData(const char* name, u32 size, u32 alignment, u32 hash, Args&& ...args)
		{
			return ReflectionDataImpl< sizeof...(Args)>
			{
				name,
					size,
					alignment,
					hash,
					std::forward<Args>(args)...
			};
		}
	}

#define VX_RF_DATA_BEGIN(TYPE) \
const auto g_rf_##TYPE \
{ \
::vx::detail::createReflectionData(#TYPE, sizeof(TYPE), __alignof(TYPE), murmurhash(#TYPE)

#define VX_RF_DATA_END(TYPE) ) };

#define VX_RF_DATA(TYPE, VALUETYPE, MEMBER) \
, ReflectionDataMember(#VALUETYPE, #MEMBER, sizeof(VALUETYPE), __alignof(VALUETYPE), offsetof(TYPE, MEMBER), murmurhash(#VALUETYPE))
}