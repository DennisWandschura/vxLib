#ifndef __STRINGID_H
#define __STRINGID_H
#pragma once

#include <vxLib/types.h>
#include <vxLib/util/CityHash.h>

namespace vx
{
	using StringID = U64;

	inline StringID make_sid(const char *str)
	{
		StringID sid = CITYHASH64(str);
		return sid;
	}

	template<size_t SIZE>
	inline StringID make_sid(const char(&str)[SIZE])
	{
		StringID sid = CityHash64((char*)str, SIZE);
		return sid;
	}

	inline StringID make_sid(const wchar_t *str)
	{
		StringID sid = CityHash64((char*)str, sizeof(wchar_t) * std::char_traits<wchar_t>::length(str));
		return sid;
	}
}

#endif