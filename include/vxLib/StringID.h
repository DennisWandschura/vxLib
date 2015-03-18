#ifndef __STRINGID_H
#define __STRINGID_H
#pragma once

#include <vxLib\types.h>
#include <vxLib\util\CityHash.h>
#include <ostream>

namespace vx
{
	struct StringID64
	{
		U64 m_value;

		friend bool operator==(const StringID64 &lhs, const StringID64 &rhs)
		{
			return lhs.m_value == rhs.m_value;
		}

		friend bool operator!=(const StringID64 &lhs, const StringID64 &rhs)
		{
			return lhs.m_value != rhs.m_value;
		}

		friend bool operator<(const StringID64 &lhs, const StringID64 &rhs)
		{
			return lhs.m_value < rhs.m_value;
		}

		friend bool operator<=(const StringID64 &lhs, const StringID64 &rhs)
		{
			return lhs.m_value <= rhs.m_value;
		}

		friend bool operator>(const StringID64 &lhs, const StringID64 &rhs)
		{
			return lhs.m_value > rhs.m_value;
		}

		friend bool operator>=(const StringID64 &lhs, const StringID64 &rhs)
		{
			return lhs.m_value >= rhs.m_value;
		}

		friend bool operator==(const StringID64 &lhs, const U64 rhs)
		{
			return lhs.m_value == rhs;
		}

		friend bool operator!=(const StringID64 &lhs, const U64 rhs)
		{
			return lhs.m_value != rhs;
		}

		friend bool operator<(const StringID64 &lhs, const U64 rhs)
		{
			return lhs.m_value < rhs;
		}

		friend bool operator<=(const StringID64 &lhs, const U64 rhs)
		{
			return lhs.m_value <= rhs;
		}

		friend bool operator>(const StringID64 &lhs, const U64 rhs)
		{
			return lhs.m_value > rhs;
		}

		friend bool operator>=(const StringID64 &lhs, const U64 rhs)
		{
			return lhs.m_value >= rhs;
		}
	};

	inline StringID64 make_sid(const U64 v)
	{
		StringID64 sid;
		sid.m_value = v;
		return sid;
	}

	inline StringID64 make_sid(const char *str)
	{
		StringID64 sid;
		sid.m_value = CITYHASH64(str);
		return sid;
	}

	template<size_t SIZE>
	inline StringID64 make_sid(const char(&str)[SIZE])
	{
		StringID64 sid;
		sid.m_value = CityHash64((char*)str, SIZE);
		return sid;
	}

	inline StringID64 make_sid(const wchar_t *str)
	{
		StringID64 sid;
		sid.m_value = CityHash64((char*)str, sizeof(wchar_t) * std::char_traits<wchar_t>::length(str));
		return sid;
	}
}

#endif