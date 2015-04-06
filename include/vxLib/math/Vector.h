#ifndef __VX_MATH_VECTOR
#define __VX_MATH_VECTOR
#pragma once

#include <vxLib/math/math.h>
#include <smmintrin.h>

#define VX_PERMUTE_PS(V, U) _mm_shuffle_ps(V, V, U)

namespace vx
{
	const U32 g_SELECT_0 = 0x00000000;
	const U32 g_SELECT_1 = 0xFFFFFFFF;

	const U32 VX_PERMUTE_0X = 0;
	const U32 VX_PERMUTE_0Y = 1;
	const U32 VX_PERMUTE_0Z = 2;
	const U32 VX_PERMUTE_0W = 3;
	const U32 VX_PERMUTE_1X = 4;
	const U32 VX_PERMUTE_1Y = 5;
	const U32 VX_PERMUTE_1Z = 6;
	const U32 VX_PERMUTE_1W = 7;

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
			vec2(value_type v) :x(v), y(v){}
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

			value_type& operator[](U32 i)
			{
				return v[i];
			}

			const value_type& operator[](U32 i) const
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
			vec2a(value_type v) :x(v), y(v){}
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

			value_type& operator[](U32 i)
			{
				return v[i];
			}

			const value_type& operator[](U32 i) const
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
			vec3(value_type v) :x(v), y(v), z(v){}
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

			value_type& operator[](U32 i)
			{
				return v[i];
			}

			const value_type& operator[](U32 i) const
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
			vec4(value_type v) :x(v), y(v), z(v), w(){}
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

			value_type& operator[](U32 i)
			{
				return v[i];
			}

			const value_type& operator[](U32 i) const
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

		template<>
		struct XMM_TYPE<F32>
		{
			typedef __m128 xmm_type;

			static xmm_type load(const vec4<F32> &src)
			{
				return _mm_loadu_ps(src.v);
			}
		};

		template<>
		struct XMM_TYPE<I32>
		{
			typedef __m128i xmm_type;

			static xmm_type load(const vec4<I32> &src)
			{
				return _mm_loadu_si128((const xmm_type*)src.v);
			}
		};

		template<>
		struct XMM_TYPE<U32>
		{
			typedef __m128i xmm_type;

			static xmm_type load(const vec4<U32> &src)
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
			I32 i[4];
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
			U32 i[4];
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

	inline __m128 VX_CALLCONV loadFloat(const detail::vec2<F32> &source)
	{
		__m128 x = _mm_load_ss(&source.x);
		__m128 y = _mm_load_ss(&source.y);
		return _mm_unpacklo_ps(x, y);
	}

	extern __m128 VX_CALLCONV loadFloat(const detail::vec2a<F32> &v);

	inline __m128 VX_CALLCONV loadFloat(const detail::vec3<F32> &source)
	{
		__m128 x = _mm_load_ss(&source.x);
		__m128 y = _mm_load_ss(&source.y);
		__m128 z = _mm_load_ss(&source.z);
		__m128 xy = _mm_unpacklo_ps(x, y);
		return _mm_movelh_ps(xy, z);
	}

	inline __m128 VX_CALLCONV loadFloat(const detail::vec4<F32> &source)
	{
		return _mm_loadu_ps(&source.x);
	}

	inline void VX_CALLCONV storeFloat(detail::vec2<F32>* pDestination, const __m128 V)
	{
		auto T = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
		_mm_store_ss(&pDestination->x, V);
		_mm_store_ss(&pDestination->y, T);
	}

	extern void VX_CALLCONV storeFloat(detail::vec2a<F32>* dst, const __m128 V);

	inline void VX_CALLCONV storeFloat(detail::vec3<F32>* pDestination, const __m128 &V)
	{
		auto T1 = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
		auto T2 = _mm_shuffle_ps(V, V, _MM_SHUFFLE(2, 2, 2, 2));
		_mm_store_ss(&pDestination->x, V);
		_mm_store_ss(&pDestination->y, T1);
		_mm_store_ss(&pDestination->z, T2);
	}

	inline void VX_CALLCONV storeFloat(detail::vec4<F32> *pDestination, const __m128 V)
	{
		_mm_storeu_ps(&pDestination->x, V);
	}

	////////////////////////
	// SSE FLOAT LOAD STORE
	////////////////////////

	////////////////////////
	// SSE INTEGER
	////////////////////////

	extern __m128i VX_CALLCONV loadInt(const I32* src);
	// stores first 32 bit int in dest
	extern void VX_CALLCONV storeInt(I32* dest, __m128i V);

	extern __m128i VX_CALLCONV loadInt(const U32* src);
	// stores first 32 bit uint in dest
	extern void VX_CALLCONV storeInt(U32* dest, __m128i V);

	inline __m128i VX_CALLCONV loadInt(const detail::vec3<I32> *pSource)
	{
		__m128i x = loadInt(&pSource->x);
		__m128i y = loadInt(&pSource->y);
		__m128i z = loadInt(&pSource->z);
		__m128i xy = _mm_unpacklo_epi32(x, z);
		return _mm_unpacklo_epi32(xy, y);
	}

	inline void VX_CALLCONV storeInt(detail::vec3<I32>* pDestination, const __m128i &V)
	{
		auto T1 = _mm_shuffle_epi32(V, _MM_SHUFFLE(1, 1, 1, 1));
		auto T2 = _mm_shuffle_epi32(V, _MM_SHUFFLE(2, 2, 2, 2));
		storeInt(&pDestination->x, V);
		storeInt(&pDestination->y, T1);
		storeInt(&pDestination->z, T2);
	}

	inline void VX_CALLCONV storeInt(detail::vec4<I32>* pDestination, const __m128i &V)
	{
		_mm_storeu_si128((__m128i*)pDestination, V);
	}

	inline __m128i VX_CALLCONV loadInt(const detail::vec3<U32> *pSource)
	{
		__m128i x = loadInt(&pSource->x);
		__m128i y = loadInt(&pSource->y);
		__m128i z = loadInt(&pSource->z);
		__m128i xy = _mm_unpacklo_epi32(x, z);
		return _mm_unpacklo_epi32(xy, y);
	}

	inline void VX_CALLCONV storeInt(detail::vec3<U32>* pDestination, const __m128i &V)
	{
		auto T1 = _mm_shuffle_epi32(V, _MM_SHUFFLE(1, 1, 1, 1));
		auto T2 = _mm_shuffle_epi32(V, _MM_SHUFFLE(2, 2, 2, 2));
		storeInt(&pDestination->x, V);
		storeInt(&pDestination->y, T1);
		storeInt(&pDestination->z, T2);
	}

	inline void VX_CALLCONV storeInt(detail::vec4<U32>* pDestination, const __m128i &V)
	{
		_mm_storeu_si128((__m128i*)pDestination, V);
	}

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

	using char2 = detail::vec2 < I8 > ;
	using char3 = detail::vec3 < I8 > ;
	using char4 = detail::vec4 < I8 > ;

	using uchar2 = detail::vec2 < U8 > ;
	using uchar3 = detail::vec3 < U8 > ;
	using uchar4 = detail::vec4 < U8 > ;

	using short2 = detail::vec2 < I16 > ;
	using short3 = detail::vec3 < I16 > ;
	using short4 = detail::vec4 < I16 > ;

	using ushort2 = detail::vec2 < U16 > ;
	using ushort3 = detail::vec3 < U16 > ;
	using ushort4 = detail::vec4 < U16 > ;

	using int2 = detail::vec2 < I32 > ;
	using int3 = detail::vec3 < I32 > ;
	using int4 = detail::vec4 < I32 > ;
	using int4a = detail::vec4a < I32 >;

	using uint2 = detail::vec2 < U32 > ;
	using uint3 = detail::vec3 < U32 > ;
	using uint4 = detail::vec4 < U32 > ;
	using uint4a = detail::vec4a < U32 >;

	using float2 = detail::vec2 < F32 > ;
	using float2a = detail::vec2a < F32 >;
	using float3 = detail::vec3 < F32 > ;
	using float3a = detail::vec3a < F32 >;
	using float4 = detail::vec4 < F32 > ;
	using float4a = detail::vec4a < F32 > ;

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

	namespace detail
	{
		extern int2 addInt2(const int2 &a, const int2 &b);
		extern uint2 addUint2(const uint2 &a, const uint2 &b);
		extern float4 addFloat4(const float4 &a, const float4 &b);
		extern float4 subFloat4(const float4 &a, const float4 &b);
		extern float4 mulFloat4(const float4 &a, const float4 &b);
		extern float4 divFloat4(const float4 &a, const float4 &b);

		extern vx::float2 min(const vx::float2 &a, const vx::float2 &b);
		extern vx::float2 max(const vx::float2 &a, const vx::float2 &b);
		extern vx::float3 min(const vx::float3 &a, const vx::float3 &b);
		extern vx::float3 max(const vx::float3 &a, const vx::float3 &b);
		extern vx::float4 min(const vx::float4 &a, const vx::float4 &b);
		extern vx::float4 max(const vx::float4 &a, const vx::float4 &b);

		extern vx::uint4 min(const vx::uint4 &a, const vx::uint4 &b);
		extern vx::uint4 max(const vx::uint4 &a, const vx::uint4 &b);

		extern vx::int4 min(const vx::int4 &a, const vx::int4 &b);
		extern vx::int4 max(const vx::int4 &a, const vx::int4 &b);

		extern vx::float4 abs(const vx::float4 &v);
		extern __m128 VX_CALLCONV abs(__m128 v);

		extern F32 dot(const vx::float2 &v1, const vx::float2 &v2);
		extern F32 dot(const vx::float3 &v1, const vx::float3 &v2);
		// unaligned float4
		extern F32 VX_CALLCONV dot(const vx::float4 &v1, const vx::float4 &v2);
		// aligned float4
		extern F32 VX_CALLCONV dot(__m128 v1, __m128 v2);

		extern F32 distance2(const vx::float2 &a, const vx::float2 &b);
		extern F32 distance2(const vx::float3 &a, const vx::float3 &b);
		extern F32 distance2(const vx::float4 &a, const vx::float4 &b);
		extern F32 VX_CALLCONV distance2(const __m128 a, const __m128 b);

		extern F32 distance(const vx::float2 &a, const vx::float2 &b);
		extern F32 distance(const vx::float3 &a, const vx::float3 &b);
		extern F32 distance(const vx::float4 &a, const vx::float4 &b);
		extern F32 VX_CALLCONV distance(__m128 a, __m128 b);

		extern F32 length(const vx::float2 &v);
		extern F32 length(const vx::float3 &v);
		extern F32 length(const vx::float4 &v);
	}

	inline float3 fma(const float3 &a, const float3 &b, const float3 &c)
	{
		return float3
			(
			::fmaf(a.x, b.x, c.x),
			::fmaf(a.y, b.y, c.y),
			::fmaf(a.z, b.z, c.z)
			);
	}

	inline float4 fma(const float4 &a, const float4 &b, const float4 &c)
	{
		auto VA = _mm_loadu_ps(a);
		auto VB = _mm_loadu_ps(b);
		auto VC = _mm_loadu_ps(c);

		auto R = _mm_fmadd_ps(VA, VB, VC);

		float4 result;
		_mm_storeu_ps(result, R);

		return result;
	}

	inline float4a VX_CALLCONV abs(float4a v)
	{
		return detail::abs(v);
	}

	template<class T>
	F32 dot(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	template<>
	F32 dot(const float2 &v1, const float2 &v2)
	{
		return detail::dot(v1, v2);
	}

	template<class T>
	F32 dot(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	template<>
	F32 dot(const float3 &v1, const float3 &v2)
	{
		return detail::dot(v1, v2);
	}

	template<class T>
	F32 dot(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	template<>
	F32 dot<F32>(const float4 &v1, const float4 &v2)
	{
		return detail::dot(v1, v2);
	}

	inline F32 VX_CALLCONV dot(float4a v1, float4a v2)
	{
		return detail::dot(v1, v2);
	}

	template<class T>
	F32 distance2(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		auto tmp = v2 - v1;
		return dot(tmp, tmp);
	}

	template<>
	F32 distance2(const float2 &v1, const float2 &v2)
	{
		return detail::distance2(v1, v2);
	}

	template<class T>
	F32 distance2(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		auto tmp = v2 - v1;
		return dot(tmp, tmp);
	}

	template<>
	F32 distance2(const float3 &v1, const float3 &v2)
	{
		return detail::distance2(v1, v2);
	}

	template<class T>
	F32 distance2(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		auto tmp = v2 - v1;
		return dot(tmp, tmp);
	}

	template<>
	F32 distance2(const float4 &v1, const float4 &v2)
	{
		return detail::distance2(v1, v2);
	}

	inline F32 VX_CALLCONV distance2(float4a v1, float4a v2)
	{
		return detail::distance2(v1.v, v2.v);
	}

	template<class T>
	F32 distance(const detail::vec2<T> &v1, const detail::vec2<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot(tmp, tmp));
	}

	template<>
	F32 distance(const float2 &v1, const float2 &v2)
	{
		return detail::distance(v1, v2);
	}

	template<class T>
	F32 distance(const detail::vec3<T> &v1, const detail::vec3<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot(tmp, tmp));
	}

	template<>
	F32 distance(const float3 &v1, const float3 &v2)
	{
		return detail::distance(v1, v2);
	}

	template<class T>
	F32 distance(const detail::vec4<T> &v1, const detail::vec4<T> &v2)
	{
		auto tmp = v2 - v1;
		return ::sqrtf(dot(tmp, tmp));
	}

	template<>
	F32 distance(const float4 &v1, const float4 &v2)
	{
		return detail::distance(v1, v2);
	}

	inline F32 VX_CALLCONV distance(float4a v1, float4a v2)
	{
		return detail::distance(v1, v2);
	}

	template<class T>
	F32 length(const detail::vec2<T> &v)
	{
		return distance(v, v);
	}

	template<>
	F32 length(const float2 &v)
	{
		return detail::length(v);
	}

	template<class T>
	F32 length(const detail::vec3<T> &v)
	{
		return distance(v, v);
	}

	template<>
	F32 length(const float3 &v)
	{
		return detail::length(v);
	}

	template<class T>
	F32 length(const detail::vec4<T> &v)
	{
		return distance(v, v);
	}

	template<>
	F32 length(const float4 &v)
	{
		return detail::length(v);
	}

	inline F32 length(float4a v)
	{
		return distance(v.v, v.v);
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

	template<typename T>
	detail::vec2<T> max(const detail::vec2<T> &a, const detail::vec2<T> &b)
	{
		return detail::vec2<T>(::std::max(a.x, b.x), ::std::max(a.y, b.y));
	}

	template<>
	float2 max(const float2 &a, const float2 &b)
	{
		return detail::max(a, b);
	}

	template<typename T>
	detail::vec3<T> max(const detail::vec3<T> &a, const detail::vec3<T> &b)
	{
		return detail::vec3<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}

	template<>
	float3 max(const float3 &a, const float3 &b)
	{
		return detail::max(a, b);
	}

	template<typename T>
	detail::vec4<T> max(const detail::vec4<T> &a, const detail::vec4<T> &b)
	{
		return detail::vec4<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
	}

	template<>
	uint4 max(const uint4 &a, const uint4 &b)
	{
		return detail::max(a, b);
	}

	template<>
	int4 max(const int4 &a, const int4 &b)
	{
		return detail::max(a, b);
	}

	template<>
	float4 max(const float4 &a, const float4 &b)
	{
		return detail::max(a, b);
	}

	template<typename T>
	detail::vec2<T> min(const detail::vec2<T> &a, const detail::vec2<T> &b)
	{
		return detail::vec2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
	}

	template<>
	float2 min(const float2 &a, const float2 &b)
	{
		return detail::min(a, b);
	}

	template<typename T>
	detail::vec3<T> min(const detail::vec3<T> &a, const detail::vec3<T> &b)
	{
		return detail::vec3<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}

	template<>
	float3 min(const float3 &a, const float3 &b)
	{
		return detail::min(a, b);
	}

	template<typename T>
	detail::vec4<T> min(const detail::vec4<T> &a, const detail::vec4<T> &b)
	{
		return detail::vec4<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
	}

	template<>
	uint4 min(const uint4 &a, const uint4 &b)
	{
		return detail::min(a, b);
	}

	template<>
	int4 min(const int4 &a, const int4 &b)
	{
		return detail::min(a, b);
	}

	template<>
	float4 min(const float4 &a, const float4 &b)
	{
		return detail::min(a, b);
	}


	inline vx::float3 degToRad(const vx::float3 &degAngle)
	{
		return  vx::float3(degAngle.x * VX_DEGTORAD, degAngle.y * VX_DEGTORAD, degAngle.z * VX_DEGTORAD);
	}

	inline __m128 degToRad(__m128 deg)
	{
		return _mm_mul_ps(deg, g_VXDegToRad);
	}

	inline __m128 radToDeg(__m128 rad)
	{
		return _mm_mul_ps(rad, g_VXRadToDeg);
	}

	// PermuteHelper internal template (SSE only)
	namespace Internal
	{
		// Slow path fallback for permutes that do not map to a single SSE shuffle opcode.
		template<U32 Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW> struct PermuteHelper
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
		template<U32 Shuffle> 
		struct PermuteHelper<Shuffle, false, false, false, false>
		{
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2) { (v2); return VX_PERMUTE_PS(v1, Shuffle); }
		};

		// Fast path for permutes that only read from the second vector.
		template<U32 Shuffle> 
		struct PermuteHelper<Shuffle, true, true, true, true>
		{
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2){ (v1); return VX_PERMUTE_PS(v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the first vector, ZW from the second.
		template<U32 Shuffle> 
		struct PermuteHelper<Shuffle, false, false, true, true>
		{
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2) { return _mm_shuffle_ps(v1, v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the second vector, ZW from the first.
		template<U32 Shuffle> struct PermuteHelper<Shuffle, true, true, false, false>
		{
			static __m128     VX_CALLCONV     Permute(__m128 v1, __m128 v2) { return _mm_shuffle_ps(v2, v1, Shuffle); }
		};
	};

	// General permute template
	template<U32 PermuteX, U32 PermuteY, U32 PermuteZ, U32 PermuteW>
	inline __m128 VX_CALLCONV VectorPermute(__m128 V1, __m128 V2)
	{
		static_assert(PermuteX <= 7, "PermuteX template parameter out of range");
		static_assert(PermuteY <= 7, "PermuteY template parameter out of range");
		static_assert(PermuteZ <= 7, "PermuteZ template parameter out of range");
		static_assert(PermuteW <= 7, "PermuteW template parameter out of range");

		const U32 Shuffle = _MM_SHUFFLE(PermuteW & 3, PermuteZ & 3, PermuteY & 3, PermuteX & 3);

		const bool WhichX = PermuteX > 3;
		const bool WhichY = PermuteY > 3;
		const bool WhichZ = PermuteZ > 3;
		const bool WhichW = PermuteW > 3;

		return Internal::PermuteHelper<Shuffle, WhichX, WhichY, WhichZ, WhichW>::Permute(V1, V2);
	}

	// Special-case permute templates
	template<> inline __m128      VX_CALLCONV     VectorPermute<0, 1, 2, 3>(__m128 V1, __m128 V2) { (V2); return V1; }
	template<> inline __m128      VX_CALLCONV     VectorPermute<4, 5, 6, 7>(__m128 V1, __m128 V2) { (V1); return V2; }

	inline bool VX_CALLCONV Vector3IsInfinite(__m128 V);
	inline bool VX_CALLCONV Vector3Equal(__m128 V1, __m128 V2);

	inline __m128 VX_CALLCONV VectorSelect(__m128 V1, __m128 V2, __m128 Control);
	inline __m128 VX_CALLCONV VectorNegate(__m128 V);

	inline __m128 VX_CALLCONV Vector3Cross(__m128 v1, __m128 v2);

	inline __m128 VX_CALLCONV Vector3Dot(__m128 V1, __m128 V2);
	inline __m128 VX_CALLCONV Vector4Dot(__m128 V1, __m128 V2);

	inline __m128 VX_CALLCONV Vector3Normalize(__m128 V);

	inline __m128 VX_CALLCONV Vector3Rotate(__m128 V, __m128 RotationQuaternion);

	inline __m128 VX_CALLCONV QuaternionMultiply(__m128 Q1, __m128 Q2);
	inline __m128 VX_CALLCONV QuaternionConjugate(__m128 Q);
	inline __m128 VX_CALLCONV QuaternionRotationRollPitchYawFromVector(__m128 vector);

	inline void VX_CALLCONV VectorSinCos(__m128* pSin, __m128* pCos, __m128 V);
	inline __m128 VX_CALLCONV VectorModAngles(__m128 Angles);

	//////////////////////// inline functions
}

//////////////////////// operator+

template<typename T>
vx::detail::vec2<T> operator+(const vx::detail::vec2<T> &lhs, const vx::detail::vec2<T> &rhs)
{
	return vx::detail::vec2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template<>
inline vx::int2 operator+(const vx::int2 &lhs, const vx::int2 &rhs)
{
	return vx::detail::addInt2(lhs, rhs);
}

template<>
inline vx::uint2 operator+(const vx::uint2 &lhs, const vx::uint2 &rhs)
{
	return vx::detail::addUint2(lhs, rhs);
}

template<typename T>
vx::detail::vec4<T> VX_CALLCONV operator + (const vx::detail::vec4<T> &lhs, const vx::detail::vec4<T> &rhs)
{
	return vx::detail::vec4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template<>
inline vx::float4 VX_CALLCONV operator + (const vx::float4 &lhs, const vx::float4 &rhs)
{
	return vx::detail::addFloat4(lhs, rhs);
}

inline vx::float4a VX_CALLCONV operator + (vx::float4a lhs, vx::float4a rhs)
{
	return _mm_add_ps(lhs.v, rhs.v);
}

//////////////////////// operator+

//////////////////////// operator-

template<typename T>
vx::detail::vec4<T> VX_CALLCONV operator - (const vx::detail::vec4<T> &lhs, const vx::detail::vec4<T> &rhs)
{
	return vx::detail::vec4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template<>
inline vx::float4 VX_CALLCONV operator - (const vx::float4 &lhs, const vx::float4 &rhs)
{
	return vx::detail::subFloat4(lhs, rhs);
}

inline vx::float4a VX_CALLCONV operator - (vx::float4a lhs, vx::float4a rhs)
{
	return _mm_sub_ps(lhs.v, rhs.v);
}

//////////////////////// operator-

//////////////////////// operator*

template<typename T>
vx::detail::vec4<T> VX_CALLCONV operator*(const vx::detail::vec4<T> &lhs, const vx::detail::vec4<T> &rhs)
{
	return vx::detail::vec4<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

template<>
inline vx::float4 VX_CALLCONV operator*(const vx::float4 &lhs, const vx::float4 &rhs)
{
	return vx::detail::mulFloat4(lhs, rhs);
}

inline vx::float4a VX_CALLCONV operator*(vx::float4a lhs, vx::float4a rhs)
{
	return _mm_mul_ps(lhs.v, rhs.v);
}

//////////////////////// operator*

//////////////////////// operator/

template<typename T>
vx::detail::vec4<T> VX_CALLCONV operator / (const vx::detail::vec4<T> &lhs, const vx::detail::vec4<T> &rhs)
{
	return vx::detail::vec4<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}

template<>
inline vx::float4 VX_CALLCONV operator / (const vx::float4 &lhs, const vx::float4 &rhs)
{
	return vx::detail::divFloat4(lhs, rhs);
}

inline vx::float4a VX_CALLCONV operator / (vx::float4a lhs, vx::float4a rhs)
{
	return _mm_div_ps(lhs.v, rhs.v);
}

//////////////////////// operator/

#include "Vector.inl"
#endif