#ifndef __VX_STREAMHELPER_H
#define __VX_STREAMHELPER_H
#pragma once

#include <istream>

namespace vx
{
	template<class T>
	bool write(std::ostream &out, const T &value)
	{
		out.write((char*)&value, sizeof(T));

		return out.good();
	}

	template<class T>
	bool write(std::ostream &out, const T *ptr, U32 count)
	{
		out.write((char*)ptr, sizeof(T) * count);

		return out.good();
	}

	template<class T>
	bool read(std::istream &is, T &value)
	{
		is.read((char*)&value, sizeof(T));

		return is.good();
	}

	template<class T>
	bool read(std::istream &is, T *ptr, U32 count)
	{
		is.read((char*)ptr, sizeof(T)*count);

		return is.good();
	}
}
#endif