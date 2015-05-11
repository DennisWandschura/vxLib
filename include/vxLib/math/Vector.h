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
#ifndef __VX_MATH_VECTOR
#define __VX_MATH_VECTOR
#pragma once

#include <vxLib/math/math.h>
#include <smmintrin.h>

#define VX_PERMUTE_PS(V, U) _mm_shuffle_ps(V, V, U)
#define _VX_DOT(M0, M1, M2, M3, X, Y, Z, W) M0 << 0 | M1 << 1 | M2 << 2 | M3 << 3 | X << 4 | Y << 5 | Z << 6 | W << 7

namespace vx
{
	const u32 g_SELECT_0 = 0x00000000;
	const u32 g_SELECT_1 = 0xFFFFFFFF;

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
		struct vec2
		{
			using value_type = T;

			union
			{
				struct
				{
					value_type x, y;
				};

				value_type v[2];
			};

			vec2() :x(), y(){}
			explicit vec2(value_type v) :x(v), y(v){}
			vec2(value_type vx, value_type vy) : x(vx), y(vy){}

			template<typename U>
			vec2(const vec2<U> &v) : x(static_cast<U>(v.x)), y(static_cast<U>(v.y)){}

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

			friend vec2 operator-(const vec2 &lhs, const vec2 &rhs)
			{
				return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
			}

			friend vec2 operator*(const vec2 &lhs, const vec2 &rhs)
			{
				return vec2(lhs.x * rhs.x, lhs.y * rhs.y);
			}

			friend vec2 operator/(const vec2 &lhs, const vec2 &rhs)
			{
				return vec2(lhs.x / rhs.x, lhs.y / rhs.y);
			}

			friend vec2 operator+(const vec2 &lhs, T rhs)
			{
				return vec2(lhs.x + rhs, lhs.y + rhs);
			}

			friend vec2 operator-(const vec2 &lhs, T rhs)
			{
				return vec2(lhs.x - rhs, lhs.y - rhs);
			}

			friend vec2 operator*(const vec2 &lhs, T rhs)
			{
				return vec2(lhs.x * rhs, lhs.y * rhs);
			}

			friend vec2 operator/(const vec2 &lhs, T rhs)
			{
				return vec2(lhs.x / rhs, lhs.y / rhs);
			}

			friend vec2 operator+(T lhs, const vec2 &rhs)
			{
				return vec2(lhs + rhs.x, lhs + rhs.y);
			}

			friend vec2 operator-(T lhs, const vec2 &rhs)
			{
				return vec2(lhs - rhs.x, lhs - rhs.y);
			}

			friend vec2 operator*(T lhs, const vec2 &rhs)
			{
				return vec2(lhs * rhs.x, lhs * rhs.y);
			}

			friend vec2 operator/(T lhs, const vec2 &rhs)
			{
				return vec2(lhs / rhs.x, lhs / rhs.y);
			}
		};

		template<class T>
		struct VX_ALIGN(8) vec2a
		{
			using value_type = T;

			union
			{
				struct
				{
					value_type x, y;
				};

				value_type v[2];
			};

			vec2a() :x(), y(){}
			explicit vec2a(value_type v) :x(v), y(v){}
			vec2a(value_type vx, value_type vy) : x(vx), y(vy){}
			vec2a(const vec2<T> &v) :x(v.x), y(v.y){}

			template<typename U>
			vec2a(const vec2a<U> &v) : x(static_cast<U>(v.x)), y(static_cast<U>(v.y)){}

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

			friend vec2a operator+(const vec2a &lhs, const vec2a &rhs)
			{
				return vec2a(lhs.x + rhs.x, lhs.y + rhs.y);
			}

			friend vec2a operator-(const vec2a &lhs, const vec2a &rhs)
			{
				return vec2a(lhs.x - rhs.x, lhs.y - rhs.y);
			}

			friend vec2a operator*(const vec2a &lhs, const vec2a &rhs)
			{
				return vec2a(lhs.x * rhs.x, lhs.y * rhs.y);
			}

			friend vec2a operator/(const vec2a &lhs, const vec2a &rhs)
			{
				return vec2a(lhs.x / rhs.x, lhs.y / rhs.y);
			}

			friend vec2a operator+(const vec2a &lhs, value_type rhs)
			{
				return vec2a(lhs.x + rhs, lhs.y + rhs);
			}

			friend vec2a operator-(const vec2a &lhs, value_type rhs)
			{
				return vec2a(lhs.x - rhs, lhs.y - rhs);
			}

			friend vec2a operator*(const vec2a &lhs, value_type rhs)
			{
				return vec2a(lhs.x * rhs, lhs.y * rhs);
			}

			friend vec2a operator/(const vec2a &lhs, value_type rhs)
			{
				return vec2a(lhs.x / rhs, lhs.y / rhs);
			}

			friend vec2a operator+(value_type lhs, const vec2a &rhs)
			{
				return vec2a(lhs + rhs.x, lhs + rhs.y);
			}

			friend vec2a operator-(value_type lhs, const vec2a &rhs)
			{
				return vec2a(lhs - rhs.x, lhs - rhs.y);
			}

			friend vec2a operator*(value_type lhs, const vec2a &rhs)
			{
				return vec2a(lhs * rhs.x, lhs * rhs.y);
			}

			friend vec2a operator/(value_type lhs, const vec2a &rhs)
			{
				return vec2a(lhs / rhs.x, lhs / rhs.y);
			}
		};

		template<class T>
		struct vec3
		{
			using value_type = T;

			union
			{
				struct
				{
					value_type x, y, z;
				};

				value_type v[3];
			};

			vec3() :x(), y(), z(){}
			explicit vec3(value_type v) :x(v), y(v), z(v){}
			vec3(value_type vx, value_type vy, value_type vz) : x(vx), y(vy), z(vz){}
			vec3(const vec2<T> &v, value_type vz) : x(v.x), y(v.y), z(vz){}
			vec3(const vec2a<T> &v, value_type vz) : x(v.x), y(v.y), z(vz){}

			template<typename U>
			vec3(const vec3<U> &v) : x(static_cast<U>(v.x)), y(static_cast<U>(v.y)), z(static_cast<U>(v.z)){}

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

			friend vec3 operator+(const vec3 &lhs, const vec3 &rhs)
			{
				return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
			}

			friend vec3 operator-(const vec3 &lhs, const vec3 &rhs)
			{
				return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
			}

			friend vec3 operator*(const vec3 &lhs, const vec3 &rhs)
			{
				return vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
			}

			friend vec3 operator/(const vec3 &lhs, const vec3 &rhs)
			{
				return vec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
			}

			friend vec3 operator+(const vec3 &lhs, T rhs)
			{
				return vec3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
			}

			friend vec3 operator-(const vec3 &lhs, T rhs)
			{
				return vec3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
			}

			friend vec3 operator*(const vec3 &lhs, T rhs)
			{
				return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
			}

			friend vec3 operator/(const vec3 &lhs, T rhs)
			{
				return vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
			}

			friend vec3 operator+(T lhs, const vec3 &rhs)
			{
				return vec3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
			}

			friend vec3 operator-(T lhs, const vec3 &rhs)
			{
				return vec3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
			}

			friend vec3 operator*(T lhs, const vec3 &rhs)
			{
				return vec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
			}

			friend vec3 operator/(T lhs, const vec3 &rhs)
			{
				return vec3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
			}
		};

		template<class T>
		struct vec4
		{
			using value_type = T;

			union
			{
				struct
				{
					value_type x, y, z, w;
				};

				value_type v[4];
			};

			vec4() :x(), y(), z(), w(){}
			explicit vec4(value_type v) :x(v), y(v), z(v), w(){}
			vec4(value_type vx, value_type vy, value_type vz, value_type vw) : x(vx), y(vy), z(vz), w(vw){}
			vec4(const vec2<T> &v, value_type vz, value_type vw) : x(v.x), y(v.y), z(vz), w(vw){}
			vec4(const vec2<T> &v1, const vec2<T> &v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y){}
			vec4(const vec3<T> &v, value_type vw) : x(v.x), y(v.y), z(v.z), w(vw){}

			template<typename U>
			vec4(const vec4<U> &v) : x(static_cast<U>(v.x)), y(static_cast<U>(v.y)), z(static_cast<U>(v.z)),w(static_cast<U>(v.w)){}

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

			inline friend vec4 operator + (const vec4 &lhs, const vec4 &rhs)
			{
				return vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
			}

			inline friend vec4 operator - (const vec4 &lhs, const vec4 &rhs)
			{
				return vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
			}

			inline friend vec4 operator * (const vec4 &lhs, const vec4 &rhs)
			{
				return vec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
			}

			inline friend vec4 operator / (const vec4 &lhs, const vec4 &rhs)
			{
				return vec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
			}
		};

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
		struct VX_ALIGN(16) vec3a
		{
			using value_type = T;
			using xmm_type = XMM_TYPE<value_type>::xmm_type;

			union
			{
				xmm_type v;
				struct
				{
					value_type x, y, z;
				};
			};

			vec3a():v(){}
			vec3a(const xmm_type &m):v(m){}

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
		struct VX_ALIGN(16) vec4a
		{
			using value_type = T;
			using xmm_type = XMM_TYPE<value_type>::xmm_type;

			union
			{
				xmm_type v;
				struct
				{
					value_type x, y, z, w;
				};
			};

			vec4a() :v(){}
			vec4a(value_type vx, value_type vy, value_type vz, value_type vw) :x(vx),y(vy),z(vz),w(vw){}
			vec4a(const vec4<T> &vu) :v(XMM_TYPE<value_type>::load(vu)){}
			vec4a(const xmm_type &m) :v(m){}

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

	struct ivec4
	{
		union
		{
			s32 i[4];
			__m128 v;
		};

		inline operator __m128() const { return v; }
		inline operator __m128i() const { return _mm_castps_si128(v); }
		inline operator __m128d() const { return _mm_castps_pd(v); }
	};

	struct uvec4
	{
		union
		{
			u32 i[4];
			__m128 v;
		};

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

	inline __m128 VX_CALLCONV loadFloat(const detail::vec2<f32> &source)
	{
		__m128 x = _mm_load_ss(&source.x);
		__m128 y = _mm_load_ss(&source.y);
		return _mm_unpacklo_ps(x, y);
	}

	inline __m128 VX_CALLCONV loadFloat(const detail::vec3<f32> &source)
	{
		__m128 x = _mm_load_ss(&source.x);
		__m128 y = _mm_load_ss(&source.y);
		__m128 z = _mm_load_ss(&source.z);
		__m128 xy = _mm_unpacklo_ps(x, y);
		return _mm_movelh_ps(xy, z);
	}

	inline __m128 VX_CALLCONV loadFloat(const detail::vec4<f32> &source)
	{
		return _mm_loadu_ps(&source.x);
	}

	inline void VX_CALLCONV storeFloat(detail::vec2<f32>* pDestination, const __m128 V)
	{
		auto T = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
		_mm_store_ss(&pDestination->x, V);
		_mm_store_ss(&pDestination->y, T);
	}

	inline void VX_CALLCONV storeFloat(detail::vec3<f32>* pDestination, const __m128 &V)
	{
		auto T1 = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
		auto T2 = _mm_shuffle_ps(V, V, _MM_SHUFFLE(2, 2, 2, 2));
		_mm_store_ss(&pDestination->x, V);
		_mm_store_ss(&pDestination->y, T1);
		_mm_store_ss(&pDestination->z, T2);
	}

	inline void VX_CALLCONV storeFloat(detail::vec4<f32> *pDestination, const __m128 V)
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

	inline __m128 VX_CALLCONV intToFloat(const __m128i V)
	{
		return _mm_castsi128_ps(V);
	}

	inline __m128i VX_CALLCONV floatToInt(const __m128 V)
	{
		return _mm_castps_si128(V);
	}

	////////////////////////

	using char2 = detail::vec2 < s8 > ;
	using char3 = detail::vec3 < s8 > ;
	using char4 = detail::vec4 < s8 > ;

	using uchar2 = detail::vec2 < u8 > ;
	using uchar3 = detail::vec3 < u8 > ;
	using uchar4 = detail::vec4 < u8 > ;

	using short2 = detail::vec2 < s16 > ;
	using short3 = detail::vec3 < s16 > ;
	using short4 = detail::vec4 < s16 > ;

	using ushort2 = detail::vec2 < u16 > ;
	using ushort3 = detail::vec3 < u16 > ;
	using ushort4 = detail::vec4 < u16 > ;

	using int2 = detail::vec2 < s32 > ;
	using int2a = detail::vec2a < s32 >;
	using int3 = detail::vec3 < s32 > ;
	using int4 = detail::vec4 < s32 > ;
	using int4a = detail::vec4a < s32 >;

	using uint2 = detail::vec2 < u32 > ;
	using uint2a = detail::vec2a < u32 >;
	using uint3 = detail::vec3 < u32 > ;
	using uint4 = detail::vec4 < u32 > ;
	using uint4a = detail::vec4a < u32 >;

	using float2 = detail::vec2 < f32 > ;
	using float2a = detail::vec2a < f32 >;
	using float3 = detail::vec3 < f32 > ;
	using float3a = detail::vec3a < f32 >;
	using float4 = detail::vec4 < f32 > ;
	using float4a = detail::vec4a < f32 > ;

	VX_GLOBALCONST __m128 g_VXDegToRad = { VX_DEGTORAD, VX_DEGTORAD, VX_DEGTORAD, VX_DEGTORAD };
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
	VX_GLOBALCONST ivec4 g_VXInfinity = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
	VX_GLOBALCONST ivec4 g_VXQNaN = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
	VX_GLOBALCONST ivec4 g_VXQNaNTest = { 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF };
	VX_GLOBALCONST ivec4 g_VXAbsMask = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
	VX_GLOBALCONST ivec4 g_VXSelect1000 = { g_SELECT_1, g_SELECT_0, g_SELECT_0, g_SELECT_0 };
	VX_GLOBALCONST ivec4 g_VXSelect1100 = { g_SELECT_1, g_SELECT_1, g_SELECT_0, g_SELECT_0 };
	VX_GLOBALCONST ivec4 g_VXSelect1110 = { g_SELECT_1, g_SELECT_1, g_SELECT_1, g_SELECT_0 };
	VX_GLOBALCONST ivec4 g_VXSelect1011 = { g_SELECT_1, g_SELECT_0, g_SELECT_1, g_SELECT_1 };
	VX_GLOBALCONST ivec4 g_VXMaskX = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };
	VX_GLOBALCONST ivec4 g_VXMaskY = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
	VX_GLOBALCONST ivec4 g_VXMaskZ = { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 };
	VX_GLOBALCONST ivec4 g_vMaskW = { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF };
	VX_GLOBALCONST ivec4 g_VXNegativeZero = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
	VX_GLOBALCONST ivec4 g_VXNegate3 = { 0x80000000, 0x80000000, 0x80000000, 0x00000000 };
	VX_GLOBALCONST ivec4 g_VXMask3 = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };

	// PermuteHelper internal template (SSE only)
	namespace Internal
	{
		// Slow path fallback for permutes that do not map to a single SSE shuffle opcode.
		template<u32 Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW> struct PermuteHelper
		{
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2)
			{
				static const uvec4 selectMask =
				{
					WhichX ? 0xFFFFFFFF : 0,
					WhichY ? 0xFFFFFFFF : 0,
					WhichZ ? 0xFFFFFFFF : 0,
					WhichW ? 0xFFFFFFFF : 0,
				};

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
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2) { (v2); return VX_PERMUTE_PS(v1, Shuffle); }
		};

		// Fast path for permutes that only read from the second vector.
		template<u32 Shuffle> 
		struct PermuteHelper<Shuffle, true, true, true, true>
		{
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2){ (v1); return VX_PERMUTE_PS(v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the first vector, ZW from the second.
		template<u32 Shuffle> 
		struct PermuteHelper<Shuffle, false, false, true, true>
		{
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2) { return _mm_shuffle_ps(v1, v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the second vector, ZW from the first.
		template<u32 Shuffle> struct PermuteHelper<Shuffle, true, true, false, false>
		{
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2) { return _mm_shuffle_ps(v2, v1, Shuffle); }
		};
	};

	// General permute template
	template<u32 PermuteX, u32 PermuteY, u32 PermuteZ, u32 PermuteW>
	inline __m128 VX_CALLCONV VectorPermute(__m128 V1, __m128 V2)
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
	template<> inline __m128      VX_CALLCONV     VectorPermute<0, 1, 2, 3>(__m128 V1, __m128 V2) { (V2); return V1; }
	template<> inline __m128      VX_CALLCONV     VectorPermute<4, 5, 6, 7>(__m128 V1, __m128 V2) { (V1); return V2; }

	inline __m128 VX_CALLCONV fma(const __m128 a, const __m128 b, const __m128 c);
	inline __m128 VX_CALLCONV dot2(const __m128 v1, const __m128 v2);
	inline __m128 VX_CALLCONV dot3(const __m128 v1, const __m128 v2);
	inline __m128 VX_CALLCONV dot4(const __m128 v1, const __m128 v2);

	inline __m128 VX_CALLCONV min(const __m128 a, const __m128 b);
	inline __m128 VX_CALLCONV max(const __m128 a, const __m128 b);

	inline __m128 VX_CALLCONV abs(const __m128 v);

	inline bool VX_CALLCONV Vector3IsInfinite(__m128 V);
	inline bool VX_CALLCONV Vector3Equal(__m128 V1, __m128 V2);

	inline __m128 VX_CALLCONV VectorSelect(__m128 V1, __m128 V2, __m128 Control);
	inline __m128 VX_CALLCONV negate(__m128 V);

	inline __m128 VX_CALLCONV cross3(__m128 v1, __m128 v2);

	inline __m128 VX_CALLCONV normalize3(__m128 V);

	inline __m128 VX_CALLCONV quaternionRotation(__m128 V, __m128 RotationQuaternion);

	inline __m128 VX_CALLCONV quaternionMultiply(__m128 Q1, __m128 Q2);
	inline __m128 VX_CALLCONV quaternionConjugate(__m128 Q);
	inline __m128 VX_CALLCONV quaternionRotationRollPitchYawFromVector(__m128 vector);

	inline void VX_CALLCONV VectorSinCos(__m128* pSin, __m128* pCos, __m128 V);
	inline __m128 VX_CALLCONV VectorModAngles(__m128 Angles);

	//////////////////////// inline functions

	template<class T>
	detail::vec2<T> min(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		return detail::vec2<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
	}

	template<class T>
	detail::vec2a<T> min(const detail::vec2a<T> &v1, const detail::vec2a<T> &v2)
	{
		return detail::vec2a<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
	}

	template<class T>
	detail::vec3<T> min(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		return detail::vec3<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}

	template<class T>
	detail::vec4<T> min(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		return detail::vec4<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z), std::min(v1.w, v2.w));
	}

	template<class T>
	detail::vec2<T> max(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		return detail::vec2<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
	}

	template<class T>
	detail::vec2a<T> max(const detail::vec2a<T> &v1, const detail::vec2a<T> &v2)
	{
		return detail::vec2a<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
	}

	template<class T>
	detail::vec3<T> max(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		return detail::vec3<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}

	template<class T>
	detail::vec4<T> max(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		return detail::vec4<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z), std::max(v1.w, v2.w));
	}

	template<class T>
	f32 dot(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	template<class T>
	f32 dot(const detail::vec2a<T> &v1, const detail::vec2a<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	template<class T>
	f32 dot(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	template<class T>
	f32 dot(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	template<>
	f32 dot<f32>(const float4 &v1, const float4 &v2)
	{
		auto a = _mm_loadu_ps(v1);
		auto b = _mm_loadu_ps(v2);
		return dot4(a, b).m128_f32[0];
	}

	inline f32 VX_CALLCONV dot(const float4a &v1, const float4a & v2)
	{
		return dot4(v1, v2).m128_f32[0];
	}

	template<class T>
	f32 distance2(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		auto tmp = v2 - v1;
		return dot(tmp, tmp);
	}

	template<class T>
	f32 distance2(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		auto tmp = v2 - v1;
		return dot(tmp, tmp);
	}

	template<class T>
	f32 distance2(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		auto tmp = v2 - v1;
		return dot(tmp, tmp);
	}

	template<class T>
	f32 distance(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot(tmp, tmp));
	}

	template<class T>
	f32 distance(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot(tmp, tmp));
	}

	template<class T>
	f32 distance(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot(tmp, tmp));
	}

	template<class T>
	f32 length(const detail::vec2<T> &v)
	{
		return distance(v, v);
	}

	template<class T>
	f32 length(const detail::vec3<T> &v)
	{
		return distance(v, v);
	}

	template<class T>
	f32 length(const detail::vec4<T> &v)
	{
		return distance(v, v);
	}

	inline float2 normalize(const float2 &v1)
	{
		auto l = sqrt(dot(v1, v1));
		float2 result(v1);
		if (l > 0.0f)
		{
			result.x /= l;
			result.y /= l;
		}

		return result;
	}

	inline float2a normalize(const float2a &v1)
	{
		auto l = sqrt(dot(v1, v1));
		float2a result(v1);
		if (l > 0.0f)
		{
			result.x /= l;
			result.y /= l;
		}

		return result;
	}

	inline float3 normalize(const float3 &v1)
	{
		auto l = sqrt(vx::dot(v1, v1));

		float3 result(v1);
		if (l > 0.0f)
		{
			result.x /= l;
			result.y /= l;
			result.z /= l;
		}

		return result;
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
}

#include "Vector.inl"
#endif