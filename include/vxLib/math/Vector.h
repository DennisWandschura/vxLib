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

#if (_VX_GCC != 0) && (_VX_ANDROID != 0)
#pragma warning( push )
#pragma warning(disable : 4201)
#endif

#include <vxLib/math/math.h>

#define VX_PERMUTE_PS(V, U) _mm_shuffle_ps(V, V, U)
#define _VX_DOT(M0, M1, M2, M3, X, Y, Z, W) M0 << 0 | M1 << 1 | M2 << 2 | M3 << 3 | X << 4 | Y << 5 | Z << 6 | W << 7

namespace vx
{
#if _VX_CALLCONV_TYPE
	typedef const __m128 CVEC4;
	typedef const __m128i CIVEC4;
#else
	typedef const __m128& CVEC4;
	typedef const __m128i& CIVEC4;
#endif

	const s32 g_SELECT_0 = 0x00000000;
	const s32 g_SELECT_1 = 0xFFFFFFFF;

	const u32 VX_PERMUTE_0X = 0;
	const u32 VX_PERMUTE_0Y = 1;
	const u32 VX_PERMUTE_0Z = 2;
	const u32 VX_PERMUTE_0W = 3;
	const u32 VX_PERMUTE_1X = 4;
	const u32 VX_PERMUTE_1Y = 5;
	const u32 VX_PERMUTE_1Z = 6;
	const u32 VX_PERMUTE_1W = 7;

	namespace detail
	{
		template<typename U>
		struct XMM_TYPE;

		template<class T>
		union vec2
		{
			typedef T value_type;

			struct
			{
				value_type x, y;
			};

			value_type v[2];

			vec2() :x(), y() {}
			explicit vec2(value_type v) :x(v), y(v) {}
			vec2(value_type vx, value_type vy) : x(vx), y(vy) {}

			template<typename U>
			vec2(const vec2<U> &v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

			operator value_type*()
			{
				return v;
			}

			operator const value_type*() const
			{
				return v;
			}

			value_type& operator[](u32 i)
			{
				return v[i];
			}

			const value_type& operator[](u32 i) const
			{
				return v[i];
			}

			vec2 operator-() const
			{
				return vec2(-x, -y);
			}

			vec2& operator+=(const vec2 &rhs)
			{
				x += rhs.x;
				y += rhs.y;

				return *this;
			}

			vec2& operator-=(const vec2 &rhs)
			{
				x -= rhs.x;
				y -= rhs.y;

				return *this;
			}

			vec2& operator*=(const vec2 &rhs)
			{
				x *= rhs.x;
				y *= rhs.y;

				return *this;
			}

			vec2& operator/=(const vec2 &rhs)
			{
				x /= rhs.x;
				y /= rhs.y;

				return *this;
			}

			vec2& operator+=(const value_type rhs)
			{
				x += rhs;
				y += rhs;

				return *this;
			}

			vec2& operator-=(const value_type rhs)
			{
				x -= rhs;
				y -= rhs;

				return *this;
			}

			vec2& operator*=(const value_type rhs)
			{
				x *= rhs;
				y *= rhs;

				return *this;
			}

			vec2& operator/=(const value_type rhs)
			{
				x /= rhs;
				y /= rhs;

				return *this;
			}
		};

		template<typename U>
		inline vec2<U> operator+(const vec2<U> &lhs, const vec2<U> &rhs)
		{
			return vec2<U>(lhs.x + rhs.x, lhs.y + rhs.y);
		}

		template<typename U>
		inline vec2<U> operator-(const vec2<U> &lhs, const vec2<U> &rhs)
		{
			return vec2<U>(lhs.x - rhs.x, lhs.y - rhs.y);
		}

		template<typename U>
		inline vec2<U> operator*(const vec2<U> &lhs, const vec2<U> &rhs)
		{
			return vec2<U>(lhs.x * rhs.x, lhs.y * rhs.y);
		}

		template<typename U>
		inline vec2<U> operator/(const vec2<U> &lhs, const vec2<U> &rhs)
		{
			return vec2<U>(lhs.x / rhs.x, lhs.y / rhs.y);
		}

		template<typename U>
		inline vec2<U> operator+(const vec2<U> &lhs, U rhs)
		{
			return vec2<U>(lhs.x + rhs, lhs.y + rhs);
		}

		template<typename U>
		inline vec2<U> operator-(const vec2<U> &lhs, U rhs)
		{
			return vec2<U>(lhs.x - rhs, lhs.y - rhs);
		}

		template<typename U>
		inline vec2<U> operator*(const vec2<U> &lhs, U rhs)
		{
			return vec2<U>(lhs.x * rhs, lhs.y * rhs);
		}

		template<typename U>
		inline vec2<U> operator/(const vec2<U> &lhs, U rhs)
		{
			return vec2<U>(lhs.x / rhs, lhs.y / rhs);
		}

		template<typename U>
		inline vec2<U> operator+(U lhs, const vec2<U> &rhs)
		{
			return vec2<U>(lhs + rhs.x, lhs + rhs.y);
		}

		template<typename U>
		inline vec2<U> operator-(U lhs, const vec2<U> &rhs)
		{
			return vec2<U>(lhs - rhs.x, lhs - rhs.y);
		}

		template<typename U>
		inline vec2<U> operator*(U lhs, const vec2<U> &rhs)
		{
			return vec2<U>(lhs * rhs.x, lhs * rhs.y);
		}

		template<typename U>
		inline vec2<U> operator/(U lhs, const vec2<U> &rhs)
		{
			return vec2<U>(lhs / rhs.x, lhs / rhs.y);
		}

		template<class T>
		union VX_ALIGN(8) vec2a
		{
			typedef T value_type;

			struct
			{
				value_type x, y;
			};

			value_type v[2];

			vec2a() :x(), y() {}
			explicit vec2a(value_type v) :x(v), y(v) {}
			vec2a(value_type vx, value_type vy) : x(vx), y(vy) {}
			vec2a(const vec2<T> &v) :x(v.x), y(v.y) {}

			template<typename U>
			vec2a(const vec2a<U> &v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

			operator value_type*()
			{
				return v;
			}

			operator const value_type*() const
			{
				return v;
			}

			value_type& operator[](u32 i)
			{
				return v[i];
			}

			const value_type& operator[](u32 i) const
			{
				return v[i];
			}

			vec2a operator-() const
			{
				return vec2a(-x, -y);
			}

			vec2a& operator+=(const vec2a &rhs)
			{
				x += rhs.x;
				y += rhs.y;

				return *this;
			}

			vec2a& operator-=(const vec2a &rhs)
			{
				x -= rhs.x;
				y -= rhs.y;

				return *this;
			}

			vec2a& operator*=(const vec2a &rhs)
			{
				x *= rhs.x;
				y *= rhs.y;

				return *this;
			}

			vec2a& operator/=(const vec2a &rhs)
			{
				x /= rhs.x;
				y /= rhs.y;

				return *this;
			}

			vec2a& operator+=(const value_type rhs)
			{
				x += rhs;
				y += rhs;

				return *this;
			}

			vec2a& operator-=(const value_type rhs)
			{
				x -= rhs;
				y -= rhs;

				return *this;
			}

			vec2a& operator*=(const value_type rhs)
			{
				x *= rhs;
				y *= rhs;

				return *this;
			}

			vec2a& operator/=(const value_type rhs)
			{
				x /= rhs;
				y /= rhs;

				return *this;
			}
		};

		template<typename U>
		inline vec2a<U> operator+(const vec2a<U> &lhs, const vec2a<U> &rhs)
		{
			return vec2a<U>(lhs.x + rhs.x, lhs.y + rhs.y);
		}

		template<typename U>
		inline vec2a<U> operator-(const vec2a<U> &lhs, const vec2a<U> &rhs)
		{
			return vec2a<U>(lhs.x - rhs.x, lhs.y - rhs.y);
		}

		template<typename U>
		inline vec2a<U> operator*(const vec2a<U> &lhs, const vec2a<U> &rhs)
		{
			return vec2a<U>(lhs.x * rhs.x, lhs.y * rhs.y);
		}

		template<typename U>
		inline vec2a<U> operator/(const vec2a<U> &lhs, const vec2a<U> &rhs)
		{
			return vec2a<U>(lhs.x / rhs.x, lhs.y / rhs.y);
		}

		template<typename U>
		inline vec2a<U> operator+(const vec2a<U> &lhs, U rhs)
		{
			return vec2a<U>(lhs.x + rhs, lhs.y + rhs);
		}

		template<typename U>
		inline vec2a<U> operator-(const vec2a<U> &lhs, U rhs)
		{
			return vec2a<U>(lhs.x - rhs, lhs.y - rhs);
		}

		template<typename U>
		inline vec2a<U> operator*(const vec2a<U> &lhs, U rhs)
		{
			return vec2a<U>(lhs.x * rhs, lhs.y * rhs);
		}

		template<typename U>
		inline vec2a<U> operator/(const vec2a<U> &lhs, U rhs)
		{
			return vec2a<U>(lhs.x / rhs, lhs.y / rhs);
		}

		template<typename U>
		inline vec2a<U> operator+(U lhs, const vec2a<U> &rhs)
		{
			return vec2a<U>(lhs + rhs.x, lhs + rhs.y);
		}

		template<typename U>
		inline vec2a<U> operator-(U lhs, const vec2a<U> &rhs)
		{
			return vec2a<U>(lhs - rhs.x, lhs - rhs.y);
		}

		template<typename U>
		inline vec2a<U> operator*(U lhs, const vec2a<U> &rhs)
		{
			return vec2a<U>(lhs * rhs.x, lhs * rhs.y);
		}

		template<typename U>
		inline vec2a<U> operator/(U lhs, const vec2a<U> &rhs)
		{
			return vec2a<U>(lhs / rhs.x, lhs / rhs.y);
		}

		template<class T>
		union vec3
		{
			typedef T value_type;

			struct
			{
				value_type x, y, z;
			};

			value_type v[3];

			vec3() :x(), y(), z() {}
			explicit vec3(value_type v) :x(v), y(v), z(v) {}
			vec3(value_type vx, value_type vy, value_type vz) : x(vx), y(vy), z(vz) {}
			vec3(const vec2<T> &v, value_type vz) : x(v.x), y(v.y), z(vz) {}
			vec3(const vec2a<T> &v, value_type vz) : x(v.x), y(v.y), z(vz) {}

			template<typename U>
			vec3(const vec3<U> &v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}

			vec3 operator-() const
			{
				return vec3(-x, -y, -z);
			}

			operator value_type*()
			{
				return v;
			}

			operator const value_type*() const
			{
				return v;
			}

			value_type& operator[](u32 i)
			{
				return v[i];
			}

			const value_type& operator[](u32 i) const
			{
				return v[i];
			}

			vec3& operator+=(const vec3 &rhs)
			{
				x += rhs.x;
				y += rhs.y;
				z += rhs.z;

				return *this;
			}

			vec3& operator-=(const vec3 &rhs)
			{
				x -= rhs.x;
				y -= rhs.y;
				z -= rhs.z;

				return *this;
			}

			vec3& operator*=(const vec3 &rhs)
			{
				x *= rhs.x;
				y *= rhs.y;
				z *= rhs.z;

				return *this;
			}

			vec3& operator/=(const vec3 &rhs)
			{
				x /= rhs.x;
				y /= rhs.y;
				z /= rhs.z;

				return *this;
			}

			vec3& operator+=(value_type rhs)
			{
				x += rhs;
				y += rhs;
				z += rhs;

				return *this;
			}

			vec3& operator-=(value_type rhs)
			{
				x -= rhs;
				y -= rhs;
				z -= rhs;

				return *this;
			}

			vec3& operator*=(value_type rhs)
			{
				x *= rhs;
				y *= rhs;
				z *= rhs;

				return *this;
			}

			vec3& operator/=(value_type rhs)
			{
				x /= rhs;
				y /= rhs;
				z /= rhs;

				return *this;
			}
		};

		template<typename U>
		inline vec3<U> operator+(const vec3<U> &lhs, const vec3<U> &rhs)
		{
			return vec3<U>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		}

		template<typename U>
		inline vec3<U> operator-(const vec3<U> &lhs, const vec3<U> &rhs)
		{
			return vec3<U>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		}

		template<typename U>
		inline vec3<U> operator*(const vec3<U> &lhs, const vec3<U> &rhs)
		{
			return vec3<U>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
		}

		template<typename U>
		inline vec3<U> operator/(const vec3<U> &lhs, const vec3<U> &rhs)
		{
			return vec3<U>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
		}

		template<typename U>
		inline vec3<U> operator+(const vec3<U> &lhs, U rhs)
		{
			return vec3<U>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
		}

		template<typename U>
		inline vec3<U> operator-(const vec3<U> &lhs, U rhs)
		{
			return vec3<U>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
		}

		template<typename U>
		inline vec3<U> operator*(const vec3<U> &lhs, U rhs)
		{
			return vec3<U>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
		}

		template<typename U>
		inline vec3<U> operator/(const vec3<U> &lhs, U rhs)
		{
			return vec3<U>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
		}

		template<typename U>
		inline vec3<U> operator+(U lhs, const vec3<U> &rhs)
		{
			return vec3<U>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
		}

		template<typename U>
		inline vec3<U> operator-(U lhs, const vec3<U> &rhs)
		{
			return vec3<U>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
		}

		template<typename U>
		inline vec3<U> operator*(U lhs, const vec3<U> &rhs)
		{
			return vec3<U>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
		}

		template<typename U>
		inline vec3<U> operator/(U lhs, const vec3<U> &rhs)
		{
			return vec3<U>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
		}

		template<class T>
		union vec4
		{
			typedef T value_type;

			struct
			{
				value_type x, y, z, w;
			};

			value_type v[4];

			vec4() :x(), y(), z(), w() {}
			explicit vec4(value_type v) :x(v), y(v), z(v), w() {}
			vec4(value_type vx, value_type vy, value_type vz, value_type vw) : x(vx), y(vy), z(vz), w(vw) {}
			vec4(const vec2<T> &v, value_type vz, value_type vw) : x(v.x), y(v.y), z(vz), w(vw) {}
			vec4(const vec2<T> &v1, const vec2<T> &v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
			vec4(const vec3<T> &v, value_type vw) : x(v.x), y(v.y), z(v.z), w(vw) {}

			template<typename U>
			vec4(const vec4<U> &v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {}

			vec4 operator-() const
			{
				return vec4(-x, -y, -z, -w);
			}

			operator value_type*()
			{
				return v;
			}

			operator const value_type*() const
			{
				return v;
			}

			value_type& operator[](u32 i)
			{
				return v[i];
			}

			const value_type& operator[](u32 i) const
			{
				return v[i];
			}

			vec4& operator+=(const vec4 &rhs)
			{
				x += rhs.x;
				y += rhs.y;
				z += rhs.z;
				w += rhs.w;

				return *this;
			}

			vec4& operator-=(const vec4 &rhs)
			{
				x -= rhs.x;
				y -= rhs.y;
				z -= rhs.z;
				w -= rhs.w;

				return *this;
			}

			vec4& operator*=(const vec4 &rhs)
			{
				x *= rhs.x;
				y *= rhs.y;
				z *= rhs.z;
				w *= rhs.w;

				return *this;
			}

			vec4& operator/=(const vec4 &rhs)
			{
				x /= rhs.x;
				y /= rhs.y;
				z /= rhs.z;
				w /= rhs.w;

				return *this;
			}
		};

		template<typename U>
		inline vec4<U> operator + (const vec4<U> &lhs, const vec4<U> &rhs)
		{
			return vec4<U>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
		}

		template<typename U>
		inline vec4<U> operator - (const vec4<U> &lhs, const vec4<U> &rhs)
		{
			return vec4<U>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
		}

		template<typename U>
		inline vec4<U> operator * (const vec4<U> &lhs, const vec4<U> &rhs)
		{
			return vec4<U>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
		}

		template<typename U>
		inline vec4<U> operator / (const vec4<U> &lhs, const vec4<U> &rhs)
		{
			return vec4<U>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
		}

		template<typename U>
		inline vec4<U> operator + (const vec4<U> &lhs, const U rhs)
		{
			return vec4<U>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
		}

		template<typename U>
		inline vec4<U> operator - (const vec4<U> &lhs, const U rhs)
		{
			return vec4<U>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
		}

		template<typename U>
		inline vec4<U> operator * (const vec4<U> &lhs, const U rhs)
		{
			return vec4<U>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
		}

		template<typename U>
		inline vec4<U> operator / (const vec4<U> &lhs, const U rhs)
		{
			return vec4<U>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
		}

		template<typename U>
		inline vec4<U> operator + (U lhs, const vec4<U> &rhs)
		{
			return vec4<U>(lhs + rhs.x, lhs + rhs.x, lhs + rhs.z, lhs + rhs.w);
		}

		template<typename U>
		inline vec4<U> operator - (U lhs, const vec4<U> &rhs)
		{
			return vec4<U>(lhs - rhs.x, lhs - rhs.x, lhs - rhs.z, lhs - rhs.w);
		}

		template<typename U>
		inline vec4<U> operator * (U lhs, const vec4<U> &rhs)
		{
			return vec4<U>(lhs * rhs.x, lhs * rhs.x, lhs * rhs.z, lhs * rhs.w);
		}

		template<typename U>
		inline vec4<U> operator / (U lhs, const vec4<U> &rhs)
		{
			return vec4<U>(lhs / rhs.x, lhs / rhs.x, lhs / rhs.z, lhs / rhs.w);
		}

		template<>
		struct XMM_TYPE<f32>
		{
			typedef __m128 xmm_type;

			static xmm_type load(const vec4<f32> &src)
			{
				return _mm_loadu_ps(src.v);
			}
		};

		template<>
		struct XMM_TYPE<s32>
		{
			typedef __m128i xmm_type;

			static xmm_type load(const vec4<s32> &src)
			{
				return _mm_loadu_si128((const xmm_type*)src.v);
			}
		};

		template<>
		struct XMM_TYPE<u32>
		{
			typedef __m128i xmm_type;

			static xmm_type load(const vec4<u32> &src)
			{
				return _mm_loadu_si128((const xmm_type*)src.v);
			}
		};

		template<typename T>
		union VX_ALIGN(16) vec3a
		{
			typedef T value_type;
			typedef typename XMM_TYPE<value_type>::xmm_type xmm_type;

			xmm_type v;
			struct
			{
				value_type x, y, z;
			};

			vec3a() :v() {}
			vec3a(const xmm_type &m) :v(m) {}

			inline operator xmm_type&()
			{
				return v;
			}

			inline operator const xmm_type&() const
			{
				return v;
			}
		};

		template<typename T>
		union VX_ALIGN(16) vec4a
		{
			typedef T value_type;
			typedef typename XMM_TYPE<value_type>::xmm_type xmm_type;

			xmm_type v;
			struct
			{
				value_type x, y, z, w;
			};

			vec4a() :v() {}
			vec4a(value_type vx, value_type vy, value_type vz, value_type vw) :x(vx), y(vy), z(vz), w(vw) {}
			vec4a(const vec4<T> &vu) :v(XMM_TYPE<value_type>::load(vu)) {}
			vec4a(const vec2a<T> &xy, const vec2a<T> &zw) :x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
			vec4a(const vec2a<T> &xy, value_type vz, value_type vw) :x(xy.x), y(xy.y), z(vz), w(vw) {}
			vec4a(const xmm_type &m) :v(m) {}

			vec4a& operator=(const vec4a &m)
			{
				v = m;
				return *this;
			}

			vec4a& operator=(const xmm_type &m)
			{
				v = m;
				return *this;
			}

			inline operator xmm_type&()
			{
				return v;
			}

			inline operator const xmm_type&() const
			{
				return v;
			}
		};
	}

#ifndef _VX_ANDROID
	union ivec4l
	{
		long long i[4];
		__m256d v;
	};
#endif

	union vector_type_int
	{
		s32 i[4];
		__m128 v;

		inline operator __m128() const { return v; }
		inline operator __m128i() const { return _mm_castps_si128(v); }
		inline operator __m128d() const { return _mm_castps_pd(v); }
	};

	union vector_type_uint
	{
		u32 i[4];
		__m128 v;

		inline operator __m128() const { return v; }
		inline operator __m128i() const { return _mm_castps_si128(v); }
		inline operator __m128d() const { return _mm_castps_pd(v); }
	};

	template<class T>
	detail::vec3<T> cross(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		return detail::vec3<T>
			(
				v1.y*v2.z - v1.z*v2.y,
				v1.z*v2.x - v1.x*v2.z,
				v1.x*v2.y - v1.y*v2.x
				);
	}

	inline __m128 VX_CALLCONV loadFloat2(const detail::vec2<f32>* source)
	{
		__m128 x = _mm_load_ss(&source->x);
		__m128 y = _mm_load_ss(&source->y);
		return _mm_unpacklo_ps(x, y);
	}

	inline __m128 VX_CALLCONV loadFloat2a(const detail::vec2a<f32>* source)
	{
		__m128i tmp = _mm_loadl_epi64((__m128i*)source);

		return *reinterpret_cast<__m128*>(&tmp);
	}

	inline __m128 VX_CALLCONV loadFloat3(const detail::vec3<f32>* source)
	{
		__m128 x = _mm_load_ss(&source->x);
		__m128 y = _mm_load_ss(&source->y);
		__m128 z = _mm_load_ss(&source->z);
		__m128 xy = _mm_unpacklo_ps(x, y);
		return _mm_movelh_ps(xy, z);
	}

	inline __m128 VX_CALLCONV loadFloat4(const detail::vec4<f32>* source)
	{
		return _mm_loadu_ps(&source->x);
	}

	inline void VX_CALLCONV storeFloat2(detail::vec2<f32>* pDestination, CVEC4 V)
	{
		__m128 T = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
		_mm_store_ss(&pDestination->x, V);
		_mm_store_ss(&pDestination->y, T);
	}

	inline void VX_CALLCONV storeFloat2a(detail::vec2a<f32>* pDestination, CVEC4 V)
	{
		_mm_storel_epi64((__m128i*)pDestination, *((__m128i*)&V));
	}

	inline void VX_CALLCONV streamFloat2a(detail::vec2a<f32>* pDestination, CVEC4 V)
	{
		_mm_stream_pd(reinterpret_cast<f64*>(pDestination), *reinterpret_cast<const __m128d*>(&V));
	}

	inline void VX_CALLCONV storeFloat3(detail::vec3<f32>* pDestination, CVEC4 V)
	{
		__m128 T1 = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 T2 = _mm_shuffle_ps(V, V, _MM_SHUFFLE(2, 2, 2, 2));
		_mm_store_ss(&pDestination->x, V);
		_mm_store_ss(&pDestination->y, T1);
		_mm_store_ss(&pDestination->z, T2);
	}

	inline void VX_CALLCONV storeFloat4(detail::vec4<f32> *pDestination, CVEC4 V)
	{
		_mm_storeu_ps(&pDestination->x, V);
	}

	////////////////////////
	// SSE FLOAT LOAD STORE
	////////////////////////

	////////////////////////
	// SSE INTEGER
	////////////////////////

	////////////////////////

	////////////////////////

	inline __m128 VX_CALLCONV intToFloat(CIVEC4 V)
	{
		return _mm_castsi128_ps(V);
	}

	inline __m128i VX_CALLCONV floatToInt(CVEC4 V)
	{
		return _mm_castps_si128(V);
	}

	////////////////////////

	typedef detail::vec2 < s8 > char2;
	typedef detail::vec3 < s8 > char3;
	typedef detail::vec4 < s8 > char4;

	typedef detail::vec2 < u8 > uchar2;
	typedef detail::vec3 < u8 > uchar3;
	typedef detail::vec4 < u8 > uchar4;

	typedef detail::vec2 < s16 > short2;
	typedef detail::vec3 < s16 > short3;
	typedef detail::vec4 < s16 > short4;

	typedef detail::vec2 < u16 > ushort2;
	typedef detail::vec3 < u16 > ushort3;
	typedef detail::vec4 < u16 > ushort4;

	typedef detail::vec2 < s32 > int2;
	typedef detail::vec2a < s32 > int2a;
	typedef detail::vec3 < s32 > int3;
	typedef detail::vec4 < s32 > int4;
	typedef detail::vec4a < s32 >  int4a;

	typedef detail::vec2 < u32 > uint2;
	typedef detail::vec2a < u32 > uint2a;
	typedef detail::vec3 < u32 > uint3;
	typedef detail::vec4 < u32 > uint4;
	typedef detail::vec4a < u32 > uint4a;

	typedef detail::vec2 < f32 > float2;
	typedef detail::vec2a < f32 > float2a;
	typedef detail::vec3 < f32 > float3;
	typedef detail::vec3a < f32 > float3a;
	typedef detail::vec4 < f32 > float4;
	typedef detail::vec4a < f32 > float4a;

	VX_GLOBALCONST __m128 g_VXDegToRad = { VX_DEGTORAD, VX_DEGTORAD, VX_DEGTORAD, VX_DEGTORAD};
	VX_GLOBALCONST __m128 g_VXRadToDeg = { VX_RADTODEG, VX_RADTODEG, VX_RADTODEG, VX_RADTODEG };
	VX_GLOBALCONST __m128 g_VXIdentityR0 = { 1.0f, 0.0f, 0.0f, 0.0f };
	VX_GLOBALCONST __m128 g_VXIdentityR1 = { 0.0f, 1.0f, 0.0f, 0.0f };
	VX_GLOBALCONST __m128 g_VXIdentityR2 = { 0.0f, 0.0f, 1.0f, 0.0f };
	VX_GLOBALCONST __m128 g_VXIdentityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	VX_GLOBALCONST __m128 g_VXNegIdentityR0 = { -1.0f, 0.0f, 0.0f, 0.0f };
	VX_GLOBALCONST __m128 g_VXNegIdentityR1 = { 0.0f, -1.0f, 0.0f, 0.0f };
	VX_GLOBALCONST __m128 g_VXNegIdentityR2 = { 0.0f, 0.0f, -1.0f, 0.0f };
	VX_GLOBALCONST __m128 g_VXNegIdentityR3 = { 0.0f, 0.0f, 0.0f, -1.0f };
	VX_GLOBALCONST __m128 g_VXNegativeTwo = { -2.0f, -2.0f, -2.0f, -2.0f };
	VX_GLOBALCONST __m128 g_VXNegativeOne = { -1.0f, -1.0f, -1.0f, -1.0f };
	VX_GLOBALCONST __m128 g_VXNegativeOneHalf = { -0.5f, -0.5f, -0.5f, -0.5f };
	VX_GLOBALCONST __m128 g_VXZero = { 0.0f, 0.0f, 0.0f, 0.0f };
	VX_GLOBALCONST __m128 g_VXOneHalf = { 0.5f, 0.5f, 0.5f, 0.5f };
	VX_GLOBALCONST __m128 g_VXOne = { 1.0f, 1.0f, 1.0f, 1.0f };
	VX_GLOBALCONST __m128 g_VXTwo = { 2.0f, 2.0f, 2.0f, 2.0f };
	VX_GLOBALCONST __m128 g_VXHalfPi = { VX_PIDIV2, VX_PIDIV2, VX_PIDIV2, VX_PIDIV2 };
	VX_GLOBALCONST __m128 g_VXPi = { VX_PI, VX_PI, VX_PI, VX_PI };
	VX_GLOBALCONST __m128 g_VXTwoPi = { VX_2PI, VX_2PI, VX_2PI, VX_2PI };
	VX_GLOBALCONST __m128 g_VXReciprocalTwoPi = { VX_1DIV2PI, VX_1DIV2PI, VX_1DIV2PI, VX_1DIV2PI };
	VX_GLOBALCONST __m128 g_VXNegateX = { -1.0f, 1.0f, 1.0f, 1.0f };
	VX_GLOBALCONST __m128 g_VXNegateY = { 1.0f, -1.0f, 1.0f, 1.0f };
	VX_GLOBALCONST __m128 g_VXNegateZ = { 1.0f, 1.0f, -1.0f, 1.0f };
	VX_GLOBALCONST __m128 g_VXNegateW = { 1.0f, 1.0f, 1.0f, -1.0f };
	VX_GLOBALCONST __m128 g_VXEpsilon = { VX_EPSILON, VX_EPSILON, VX_EPSILON, VX_EPSILON };
	VX_GLOBALCONST __m128 g_VXSinCoefficients0 = { -0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f };
	VX_GLOBALCONST __m128 g_VXSinCoefficients1 = { -2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/ };
	VX_GLOBALCONST __m128 g_VXCosCoefficients0 = { -0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f };
	VX_GLOBALCONST __m128 g_VXCosCoefficients1 = { -2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ };
	VX_GLOBALCONST __m128 g_VXATanCoefficients0 = { -0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f };
	VX_GLOBALCONST __m128 g_VXATanCoefficients1 = { -0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f };
	VX_GLOBALCONST __m128 g_VXNoFraction = { 8388608.0f,8388608.0f,8388608.0f,8388608.0f };
	VX_GLOBALCONST vector_type_int g_VXInfinity = { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } };
	VX_GLOBALCONST vector_type_int g_VXQNaN = { { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 } };
	VX_GLOBALCONST vector_type_int g_VXQNaNTest = { { 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF } };
	VX_GLOBALCONST vector_type_int g_VXAbsMask = { { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF } };
	VX_GLOBALCONST vector_type_int g_VXSelect1000 = { { g_SELECT_1, g_SELECT_0, g_SELECT_0, g_SELECT_0 } };
	VX_GLOBALCONST vector_type_int g_VXSelect1100 = { { g_SELECT_1, g_SELECT_1, g_SELECT_0, g_SELECT_0 } };
	VX_GLOBALCONST vector_type_int g_VXSelect1110 = { { g_SELECT_1, g_SELECT_1, g_SELECT_1, g_SELECT_0 } };
	VX_GLOBALCONST vector_type_int g_VXSelect1011 = { { g_SELECT_1, g_SELECT_0, g_SELECT_1, g_SELECT_1 } };
	VX_GLOBALCONST vector_type_int g_VXMaskX = { { (s32)0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 } };
	VX_GLOBALCONST vector_type_int g_VXMaskY = { { 0x00000000, (s32)0xFFFFFFFF, 0x00000000, 0x00000000 } };
	VX_GLOBALCONST vector_type_int g_VXMaskZ = { { 0x00000000, 0x00000000, (s32)0xFFFFFFFF, 0x00000000 } };
	VX_GLOBALCONST vector_type_int g_VXMaskW = { { 0x00000000, 0x00000000, 0x00000000, (s32)0xFFFFFFFF } };
	VX_GLOBALCONST vector_type_int g_VXNegativeZero = { { (s32)0x80000000, (s32)0x80000000, (s32)0x80000000, (s32)0x80000000 } };
	VX_GLOBALCONST vector_type_int g_VXNegate3 = { { (s32)0x80000000, (s32)0x80000000, (s32)0x80000000, 0x00000000 } };
	VX_GLOBALCONST vector_type_int g_VXMask3 = { { (s32)0xFFFFFFFF, (s32)0xFFFFFFFF, (s32)0xFFFFFFFF, 0x00000000 } };
	VX_GLOBALCONST vector_type_int g_VXMask4 = { { (s32)0xFFFFFFFF, (s32)0xFFFFFFFF, (s32)0xFFFFFFFF, (s32)0xFFFFFFFF } };

#ifndef _VX_ANDROID
	VX_GLOBALCONST ivec4l g_VXInfinity_d = { 0x7FF0000000000000, 0x7FF0000000000000, 0x7FF0000000000000, 0x7FF0000000000000 };
	VX_GLOBALCONST ivec4l g_VXQNaN_d = { 0x7FF8000000000000, 0x7FF8000000000000, 0x7FF8000000000000, 0x7FF8000000000000 };
	VX_GLOBALCONST ivec4l g_VXSelect1110_d = { (s64)0xFFFFFFFFFFFFFFFF, (s64)0xFFFFFFFFFFFFFFFF, (s64)0xFFFFFFFFFFFFFFFF, 0 };
	VX_GLOBALCONST ivec4l g_VXMask3_d = { (s64)0xFFFFFFFFFFFFFFFF, (s64)0xFFFFFFFFFFFFFFFF, (s64)0xFFFFFFFFFFFFFFFF, 0x0000000000000000 };
	VX_GLOBALCONST ivec4l g_VXMaskY_d = { 0, (s64)0xffffffffffffffff, 0, 0 };
#endif

	// PermuteHelper internal template (SSE only)
	namespace Internal
	{
		// Slow path fallback for permutes that do not map to a single SSE shuffle opcode.
		template<u32 Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW> struct PermuteHelper
		{
			static __m128     VX_CALLCONV     Permute(CVEC4 v1, CVEC4 v2)
			{
				static const vector_type_uint selectMask =
				{ {
					WhichX ? 0xFFFFFFFF : 0,
					WhichY ? 0xFFFFFFFF : 0,
					WhichZ ? 0xFFFFFFFF : 0,
					WhichW ? 0xFFFFFFFF : 0,
				} };

				__m128 shuffled1 = VX_PERMUTE_PS(v1, Shuffle);
				__m128 shuffled2 = VX_PERMUTE_PS(v2, Shuffle);

				__m128 masked1 = _mm_andnot_ps(selectMask, shuffled1);
				__m128 masked2 = _mm_and_ps(selectMask, shuffled2);

				return _mm_or_ps(masked1, masked2);
			}
		};

		// Fast path for permutes that only read from the first vector.
		template<u32 Shuffle>
		struct PermuteHelper<Shuffle, false, false, false, false>
		{
			static __m128     VX_CALLCONV     Permute(CVEC4 v1, CVEC4) { return VX_PERMUTE_PS(v1, Shuffle); }
		};

		// Fast path for permutes that only read from the second vector.
		template<u32 Shuffle>
		struct PermuteHelper<Shuffle, true, true, true, true>
		{
			static __m128     VX_CALLCONV     Permute(CVEC4, CVEC4 v2) { return VX_PERMUTE_PS(v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the first vector, ZW from the second.
		template<u32 Shuffle>
		struct PermuteHelper<Shuffle, false, false, true, true>
		{
			static __m128     VX_CALLCONV     Permute(CVEC4 v1, CVEC4 v2) { return _mm_shuffle_ps(v1, v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the second vector, ZW from the first.
		template<u32 Shuffle> struct PermuteHelper<Shuffle, true, true, false, false>
		{
			static __m128     VX_CALLCONV     Permute(CVEC4 v1, CVEC4 v2) { return _mm_shuffle_ps(v2, v1, Shuffle); }
		};
	};

	// General permute template
	template<u32 PermuteX, u32 PermuteY, u32 PermuteZ, u32 PermuteW>
	inline __m128 VX_CALLCONV VectorPermute(CVEC4 V1, CVEC4 V2)
	{
		static_assert(PermuteX <= 7, "PermuteX template parameter out of range");
		static_assert(PermuteY <= 7, "PermuteY template parameter out of range");
		static_assert(PermuteZ <= 7, "PermuteZ template parameter out of range");
		static_assert(PermuteW <= 7, "PermuteW template parameter out of range");

		const u32 Shuffle = _MM_SHUFFLE(PermuteW & 3, PermuteZ & 3, PermuteY & 3, PermuteX & 3);

		const bool WhichX = PermuteX > 3;
		const bool WhichY = PermuteY > 3;
		const bool WhichZ = PermuteZ > 3;
		const bool WhichW = PermuteW > 3;

		return Internal::PermuteHelper<Shuffle, WhichX, WhichY, WhichZ, WhichW>::Permute(V1, V2);
	}

	// Special-case permute templates
	template<> inline __m128      VX_CALLCONV     VectorPermute<0, 1, 2, 3>(CVEC4 V1, CVEC4) { return V1; }
	template<> inline __m128      VX_CALLCONV     VectorPermute<4, 5, 6, 7>(CVEC4, CVEC4 V2) { return V2; }

	inline __m128 VX_CALLCONV fma(CVEC4 a, CVEC4 b, CVEC4 c);
	inline __m128 VX_CALLCONV VectorNegativeMultiplySubtract(CVEC4 a, CVEC4 b, CVEC4 c);
	inline __m128 VX_CALLCONV dot2(CVEC4 v1, CVEC4 v2);
	inline __m128 VX_CALLCONV dot3(CVEC4 v1, CVEC4 v2);
	inline __m128 VX_CALLCONV dot4(CVEC4 v1, CVEC4 v2);

	inline __m128 VX_CALLCONV min(CVEC4 a, CVEC4 b);
	inline __m128 VX_CALLCONV max(CVEC4 a, CVEC4 b);

	inline __m128 VX_CALLCONV abs(CVEC4 v);

	inline bool VX_CALLCONV Vector3IsInfinite(CVEC4 V);
	inline bool VX_CALLCONV Vector3Equal(CVEC4 V1, CVEC4 V2);
	inline __m128 VX_CALLCONV VectorInBounds(CVEC4 V, CVEC4 Bounds);

	inline __m128 VX_CALLCONV VectorTrueInt();
	inline __m128 VX_CALLCONV VectorAndInt(CVEC4 V1, CVEC4 V2);
	inline __m128 VX_CALLCONV VectorEqualInt(CVEC4 V1, CVEC4 V2);
	inline __m128 VX_CALLCONV VectorOrInt(CVEC4 V1, CVEC4 V2);
	inline __m128 VX_CALLCONV VectorIsInfinite(CVEC4 V);

	inline __m128 VX_CALLCONV VectorSelect(CVEC4 V1, CVEC4 V2, CVEC4 Control);
	inline __m128 VX_CALLCONV negate(CVEC4 V);
	inline __m128 VX_CALLCONV round(CVEC4 V);

	inline __m128 VX_CALLCONV cross3(CVEC4 v1, CVEC4 v2);

	inline __m128 VX_CALLCONV length2(CVEC4 V);
	inline __m128 VX_CALLCONV length3(CVEC4 V);
	inline __m128 VX_CALLCONV length4(CVEC4 V);

	inline __m128 VX_CALLCONV normalize2(CVEC4 V);
	inline __m128 VX_CALLCONV normalize3(CVEC4 V);
	inline __m128 VX_CALLCONV normalize4(CVEC4 V);

	inline __m128 VX_CALLCONV quaternionRotation(CVEC4 V, CVEC4 RotationQuaternion);

	inline __m128 VX_CALLCONV quaternionMultiply(CVEC4 Q1, CVEC4 Q2);
	inline __m128 VX_CALLCONV quaternionConjugate(CVEC4 Q);
	inline __m128 VX_CALLCONV quaternionRotationRollPitchYawFromVector(CVEC4 vector);
	inline void VX_CALLCONV quaternionToAxisAngle(CVEC4 Q, __m128* pAxis, f32* pAngle);

	inline void VX_CALLCONV sinCos(__m128* pSin, __m128* pCos, CVEC4 V);
	inline __m128 VX_CALLCONV tan(CVEC4 V);
	inline __m128 VX_CALLCONV atan(CVEC4 V);
	inline __m128 VX_CALLCONV atan2(CVEC4 x, CVEC4 y);
	inline __m128 VX_CALLCONV modAngles(CVEC4 Angles);

	inline f32 VX_CALLCONV VectorGetW(CVEC4 V);

	inline __m128 VX_CALLCONV VectorSplatX(CVEC4 V);
	inline __m128 VX_CALLCONV VectorSplatY(CVEC4 V);
	inline __m128 VX_CALLCONV VectorSplatZ(CVEC4 V);
	inline __m128 VX_CALLCONV VectorSplatW(CVEC4 V);

	//////////////////////// inline functions

	template<class T>
	inline detail::vec2<T> min(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		return detail::vec2<T>(vx::min(v1.x, v2.x), vx::min(v1.y, v2.y));
	}

	template<>
	inline detail::vec2<f32> min(const detail::vec2<f32> &v1, const detail::vec2<f32> &v2)
	{
		return detail::vec2<f32>(fminf(v1.x, v2.x), fminf(v1.y, v2.y));
	}

	template<class T>
	inline detail::vec2a<T> min(const detail::vec2a<T> &v1, const detail::vec2a<T> &v2)
	{
		return detail::vec2a<T>(vx::min(v1.x, v2.x), vx::min(v1.y, v2.y));
	}

	template<>
	inline detail::vec2a<f32> min(const detail::vec2a<f32> &v1, const detail::vec2a<f32> &v2)
	{
		__m128 r = _mm_min_ps(vx::loadFloat2a(&v1), vx::loadFloat2a(&v2));
		detail::vec2a<f32> result;
		vx::storeFloat2a(&result, r);
		return result;
	}

	template<class T>
	inline detail::vec3<T> min(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		return detail::vec3<T>(vx::min(v1.x, v2.x), vx::min(v1.y, v2.y), vx::min(v1.z, v2.z));
	}

	template<>
	inline detail::vec3<f32> min(const detail::vec3<f32> &v1, const detail::vec3<f32> &v2)
	{
		return detail::vec3<f32>(fminf(v1.x, v2.x), fminf(v1.y, v2.y), fminf(v1.z, v2.z));
	}

	template<class T>
	inline detail::vec4<T> min(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		return detail::vec4<T>(vx::min(v1.x, v2.x), vx::min(v1.y, v2.y), vx::min(v1.z, v2.z), vx::min(v1.w, v2.w));
	}

	template<class T>
	inline detail::vec2<T> max(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		return detail::vec2<T>(vx::max(v1.x, v2.x), vx::max(v1.y, v2.y));
	}

	template<>
	inline detail::vec2<f32> max(const detail::vec2<f32> &v1, const detail::vec2<f32> &v2)
	{
		return detail::vec2<f32>(fmaxf(v1.x, v2.x), fmaxf(v1.y, v2.y));
	}

	template<class T>
	inline detail::vec2a<T> max(const detail::vec2a<T> &v1, const detail::vec2a<T> &v2)
	{
		return detail::vec2a<T>(vx::max(v1.x, v2.x), vx::max(v1.y, v2.y));
	}

	template<>
	inline detail::vec2a<f32> max(const detail::vec2a<f32> &v1, const detail::vec2a<f32> &v2)
	{
		__m128 r = _mm_max_ps(vx::loadFloat2a(&v1), vx::loadFloat2a(&v2));
		detail::vec2a<f32> result;
		vx::storeFloat2a(&result, r);
		return result;
	}

	template<class T>
	inline detail::vec3<T> max(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		return detail::vec3<T>(vx::max(v1.x, v2.x), vx::max(v1.y, v2.y), vx::max(v1.z, v2.z));
	}

	template<class T>
	inline detail::vec4<T> max(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		return detail::vec4<T>(vx::max(v1.x, v2.x), vx::max(v1.y, v2.y), vx::max(v1.z, v2.z), vx::max(v1.w, v2.w));
	}

	template<class T>
	inline f32 dot2(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	template<class T>
	inline f32 dot2(const detail::vec2a<T> &v1, const detail::vec2a<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	template<class T>
	inline f32 dot3(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	template<class T>
	inline f32 dot4(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	template<>
	inline f32 dot4<f32>(const float4 &v1, const float4 &v2)
	{
		__m128 a = _mm_loadu_ps(v1);
		__m128 b = _mm_loadu_ps(v2);

		__m128 tmp = dot4(a, b);

		f32 result;
		_mm_store_ss(&result, tmp);
		return result;
	}

	inline f32 VX_CALLCONV dot4(const float4a &v1, const float4a &v2)
	{
		__m128 tmp = dot4(v1.v, v2.v);

		f32 result;
		_mm_store_ss(&result, tmp);
		return result;
	}

	template<class T>
	f32 distance2(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot2(tmp, tmp));
	}

	template<class T>
	f32 distance2(const detail::vec2a<T> &v1, const detail::vec2a<T> &v2)
	{
		auto a = vx::loadFloat2a(&v1);
		auto b = vx::loadFloat2a(&v2);
		auto d = _mm_sub_ps(b, a);
		d = vx::length2(d);

		return d.m128_f32[0];
	}

	template<class T>
	f32 distance3(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot(tmp, tmp));
	}

	template<>
	inline f32 distance3(const detail::vec3<f32> &v1, const detail::vec3<f32> &v2)
	{
		__m128 a = vx::loadFloat3(&v1);
		__m128 b = vx::loadFloat3(&v2);
		__m128 d = _mm_sub_ps(b, a);
		d = dot3(d, d);
		d = _mm_sqrt_ps(d);

                return *reinterpret_cast<f32*>(&d);
	}

	template<class T>
	f32 distance4(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot(tmp, tmp));
	}

	template<>
	inline f32 distance4(const detail::vec4<f32> &v1, const detail::vec4<f32> &v2)
	{
		__m128 a = vx::loadFloat4(&v1);
		__m128 b = vx::loadFloat4(&v2);
		__m128 d = _mm_sub_ps(b, a);
		d = dot4(d, d);
		d = _mm_sqrt_ps(d);

                return *reinterpret_cast<f32*>(&d);
	}

	template<class T>
	f32 length2(const detail::vec2<T> &v)
	{
		return sqrtf(dot2(v, v));
	}

	template<class T>
	f32 length2(const detail::vec2a<T> &v)
	{
		auto a = vx::loadFloat2a(&v);
		return length2(a).m128_f32[0];
	}

	template<class T>
	f32 length3(const detail::vec3<T> &v)
	{
		return sqrtf(dot3(v, v));
	}

	template<class T>
	f32 length4(const detail::vec4<T> &v)
	{
		return sqrtf(dot4(v, v));
	}

	inline float2 normalize2(const float2 &v1)
	{
		auto l = sqrtf(dot2(v1, v1));
		float2 result(v1);
		if (l > 0.0f)
		{
			result.x /= l;
			result.y /= l;
		}

		return result;
	}

	inline float2a normalize2(const float2a &v1)
	{
		f32 l = sqrtf(dot2(v1, v1));
		float2a result(v1);
		if (l > 0.0f)
		{
			result.x /= l;
			result.y /= l;
		}

		return result;
	}

	inline float3 normalize3(const float3 &v1)
	{
		f32 l = sqrtf(vx::dot3(v1, v1));

		float3 result(v1);
		if (l > 0.0f)
		{
			result.x /= l;
			result.y /= l;
			result.z /= l;
		}

		return result;
	}

	template<class T>
	inline detail::vec2<T> abs(const detail::vec2<T> &v)
	{
                return detail::vec2<T>(vx::abs(v.x), vx::abs(v.y));
	}

	template<>
	inline float2 abs(const float2 &v)
	{
		return float2(vx::abs(v.x), vx::abs(v.y));
	}

	template<class T>
	inline detail::vec2a<T> abs(const detail::vec2a<T> &v)
	{
                return detail::vec2a<T>(vx::abs(v.x), vx::abs(v.y));
	}

	template<>
	inline float2a abs(const float2a &v)
	{
		__m128 r = vx::abs(vx::loadFloat2a(&v));
		vx::float2a result;
		vx::storeFloat2a(&result, r);
		return result;
	}

	template<class T>
	inline detail::vec3<T> abs(const detail::vec3<T> &v)
	{
		return detail::vec3<T>(abs(v.x), abs(v.y), abs(v.z));
	}

	template<>
	inline float3 abs(const float3 &v)
	{
		return float3(fabsf(v.x), fabsf(v.y), fabsf(v.z));
	}

	template<class T>
	inline detail::vec4<T> abs(const detail::vec4<T> &v)
	{
		return detail::vec4<T>(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
	}

	template<>
	inline float4 abs(const float4 &v)
	{
		__m128 tmp = vx::loadFloat4(&v);
		__m128 abs = vx::abs(tmp);

		vx::float4 result;
		vx::storeFloat4(&result, abs);
		return result;
	}

	inline float2 round(const float2 &v)
	{
		__m128 tmp = vx::loadFloat2(&v);
		tmp = round(tmp);

		vx::float2 result;
		vx::storeFloat2(&result, tmp);
		return result;
	}

	inline float2a round(const float2a &v)
	{
		__m128 tmp = vx::loadFloat2a(&v);
		tmp = round(tmp);

		vx::float2a result;
		vx::storeFloat2a(&result, tmp);
		return result;
	}

	inline float3 round(const float3 &v)
	{
		return float3(roundf(v.x), roundf(v.y), roundf(v.z));
	}

	inline float4 round(const float4 &v)
	{
		__m128 tmp = vx::loadFloat4(&v);
		tmp = round(tmp);

		vx::float4 result;
		vx::storeFloat4(&result, tmp);
		return result;
	}

	inline float4a round(const float4a &v)
	{
		return round(v.v);
	}

	inline vx::float3 degToRad(const vx::float3 &degAngle)
	{
		return vx::float3(degAngle.x * VX_DEGTORAD, degAngle.y * VX_DEGTORAD, degAngle.z * VX_DEGTORAD);
	}

	inline __m128 degToRad(__m128 deg)
	{
		return _mm_mul_ps(deg, g_VXDegToRad);
	}

	inline __m128 radToDeg(__m128 rad)
	{
		return _mm_mul_ps(rad, g_VXRadToDeg);
	}

	inline vx::float3 radToDeg(const vx::float3 &radAngle)
	{
		return vx::float3(radAngle.x * VX_RADTODEG, radAngle.y * VX_RADTODEG, radAngle.z * VX_RADTODEG);
	}

	inline void angleAxisToEuler(const vx::float4a &normalizedAxis, f32 angle, vx::float3* rollPitchYaw)
	{
		f32 s = sinf(angle);
		f32 c = cosf(angle);
		f32 t = 1.0f - c;

		f32 pitch = 0, yaw = 0, roll = 0;

		f32 tmp = (normalizedAxis.x * normalizedAxis.y * t + normalizedAxis.z * s);
		if (tmp > 0.998f)
		{ // north pole singularity detected
			pitch = 2.0f * ::atan2f(normalizedAxis.x * sinf(angle / 2.0f), cosf(angle / 2.0f));
			yaw = vx::VX_PI / 2.0f;
			roll = 0.0f;
		}
		else if (tmp < -0.998f)
		{ // south pole singularity detected
			pitch = -2.0f * ::atan2f(normalizedAxis.x * sinf(angle / 2.0f), cosf(angle / 2.0f));
			yaw = -vx::VX_PI / 2.0f;
			roll = 0.0f;
		}
		else
		{
			pitch = ::atan2f(normalizedAxis.y * s - normalizedAxis.x * normalizedAxis.z * t, 1.0f - (normalizedAxis.y * normalizedAxis.y + normalizedAxis.z * normalizedAxis.z) * t);
			yaw = ::asinf(normalizedAxis.x * normalizedAxis.y * t + normalizedAxis.z * s);
			roll = ::atan2f(normalizedAxis.x * s - normalizedAxis.y * normalizedAxis.z * t, 1.0f - (normalizedAxis.x * normalizedAxis.x + normalizedAxis.z * normalizedAxis.z) * t);
		}

		rollPitchYaw->x = radToDeg(roll);
		rollPitchYaw->y = radToDeg(pitch);
		rollPitchYaw->z = radToDeg(yaw);
	}

	template<typename T>
	inline vx::detail::vec2<T> clamp(const vx::detail::vec2<T> &value, const vx::detail::vec2<T> &vmin, const vx::detail::vec2<T> &vmax)
	{
		return min(max(value, vmin), vmax);
	}

	template<typename T>
	inline vx::detail::vec2a<T> clamp(const vx::detail::vec2a<T> &value, const vx::detail::vec2a<T> &vmin, const vx::detail::vec2a<T> &vmax)
	{
		return min(max(value, vmin), vmax);
	}

	template<typename T>
	inline vx::detail::vec3<T> clamp(const vx::detail::vec3<T> &value, const vx::detail::vec3<T> &vmin, const vx::detail::vec3<T> &vmax)
	{
		return min(max(value, vmin), vmax);
	}

	template<typename T>
	inline vx::detail::vec4<T> clamp(const vx::detail::vec4<T> &value, const vx::detail::vec4<T> &vmin, const vx::detail::vec4<T> &vmax)
	{
		return min(max(value, vmin), vmax);
	}
}

#include "Vector.inl"

#if (_VX_GCC != 0 && _VX_ANDROID != 0)
#pragma warning( pop )
#endif
