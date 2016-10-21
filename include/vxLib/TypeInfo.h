#pragma once

/*
The MIT License (MIT)

Copyright (c) 2016 Dennis Wandschura

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

#include <vxLib/Container/StringArray.h>
#include <vxLib/math/Vector.h>
#include <vxLib/StringID.h>
#include <memory>
#include <vxLib/hash.h>

namespace vx
{
	namespace detail
	{
		template<unsigned...>struct seq { using type = seq; };

		template<unsigned N, unsigned... Is>
		struct gen_seq_x : gen_seq_x<N - 1, N - 1, Is...> {};

		template<unsigned... Is>
		struct gen_seq_x<0, Is...> : seq<Is...> {};

		template<unsigned N>
		using gen_seq = typename gen_seq_x<N>::type;
	}

	template<size_t N>
	struct StringBuffer
	{
		typedef char value_type;
		typedef char* pointer;
		typedef const char* const_pointer;

		char m_buffer[N];

		template<size_t LEN, unsigned...I1>
		constexpr StringBuffer(const char(&str)[LEN], detail::seq<I1...>) :m_buffer{ str[I1]..., '\0' } {}

		template<size_t LEN>
		constexpr StringBuffer(const char(&str)[LEN]) : StringBuffer(str, detail::gen_seq<LEN - 1>{}) {}

		constexpr char operator[](size_t index) const { return m_buffer[index]; }

		constexpr const_pointer c_str() const { return reinterpret_cast<const_pointer>(m_buffer); }
		constexpr const_pointer data() const { return reinterpret_cast<const_pointer>(m_buffer); }

		constexpr size_t size() const { return (N - 1); }

		constexpr const_pointer begin() const { return m_buffer; }
		pointer begin() { return m_buffer; }

		constexpr const_pointer end() const { return m_buffer + N - 1; }
		pointer end() { return m_buffer + N - 1; }
	};

	struct TypeInfoBase
	{
		const char* m_ptr;
		size_t m_size;
		size_t m_alignment;
		u32 m_hash;
		const TypeInfoBase* m_parent;

		constexpr TypeInfoBase(const char* ptr, size_t size, size_t alignment, u32 hash, const TypeInfoBase* parent) :m_ptr(ptr), m_size(size), m_alignment(alignment), m_hash(hash), m_parent(parent) {}

		constexpr const char* getName() const { return m_ptr; }
		constexpr u32 getHash() const { return m_hash; }
	};

	template<size_t SZ>
	struct TypeInfo : public TypeInfoBase
	{
		StringBuffer<SZ> m_name;

		template<size_t LEN>
		constexpr TypeInfo(const char(&str)[LEN], size_t size, size_t alignment, u32 hash, const TypeInfoBase* parent = nullptr) : m_name(str, detail::gen_seq<LEN - 1>{}), TypeInfoBase(m_name.m_buffer, size, alignment, hash, parent) {}
	};

	namespace detail
	{
		template<class ...Args>
		struct Length;

		template<class T, class ...Args>
		struct Length<T, Args...>
		{
			enum { value = Length<T>::value + Length<Args...>::value };
		};

		template<class T, size_t N>
		struct Length<T[N]>
		{
			enum { value = N - 1 };
		};

		template<class T, size_t N>
		struct Length<std::array<T, N>>
		{
			enum { value = N - 1 };
		};

		template<size_t N>
		struct Length<StringBuffer<N>>
		{
			enum { value = N - 1 };
		};

		template<>
		struct Length<char>
		{
			enum { value = 1 };
		};

		template<>
		struct Length<>
		{
			enum { value = 0 };
		};

		struct GetLength
		{
			template<class T>
			constexpr static size_t value(const T&l)
			{
				return Length<T>::value(l);
			}

			template<class T, class R>
			constexpr static size_t value(const T&l, const R&r)
			{
				return Length<T>::value(l) + Length<R>::value(r);
			}
		};

		template<class...Ts>
		using combined_string = StringBuffer<Length<Ts...>::value + 1 >;

		template<class T0, class T1, unsigned...I1, unsigned...I2>
		constexpr combined_string<T0, T1>
			concat_impl(const T0 &lhs, const T1 &rhs, seq<I1...>, seq<I2...>)
		{
			return{ { lhs[I1]..., rhs[I2]..., '\0' } };
		}

		template<class Lhs, class Rhs>
		constexpr const StringBuffer<Length<Lhs, Rhs>::value + 1>
			concat(const Lhs &lhs, const Rhs &rhs)
		{
			return concat_impl(lhs, rhs,
				gen_seq<Length<Lhs>::value>{},
				gen_seq<Length<Rhs>::value>{});
		}

		template<class T0, class T1, class... Ts>
		constexpr const combined_string<T0, T1, Ts...>
			concat(T0 const&t0, T1 const&t1, Ts const&...ts)
		{
			return concat(t0, concat(t1, ts...));
		}

		template<bool, typename T>
		struct TypeInfo_SizeGetterImpl
		{
			enum { size = 0, alignment = 0 };
		};

		template<typename T>
		struct TypeInfo_SizeGetterImpl<false, T>
		{
			enum { size = sizeof(T), alignment = __alignof(T) };
		};

		template<typename T>
		struct TypeInfo_SizeGetter
		{
			enum 
			{
				size = TypeInfo_SizeGetterImpl<std::is_abstract<T>::value, T>::size, 
				alignment = TypeInfo_SizeGetterImpl<std::is_abstract<T>::value, T>::alignment
			};
		};

		template<size_t LEN>
		constexpr TypeInfo<LEN> getTypeInfo(const char(&name)[LEN], size_t size, size_t alignment)
		{
			return TypeInfo<LEN>{name, size, alignment, murmurhash(name) };
		}

		template<typename T, size_t LEN>
		constexpr TypeInfo<LEN> getTypeInfo(const char(&name)[LEN])
		{
			return TypeInfo<LEN>{name, TypeInfo_SizeGetter<T>::size, TypeInfo_SizeGetter<T>::alignment, murmurhash(name) };
		}

		template<typename T, size_t LEN>
		constexpr TypeInfo<LEN> getTypeInfo(const StringBuffer<LEN> &name)
		{
			return TypeInfo<LEN>{name.m_buffer, TypeInfo_SizeGetter<T>::size, TypeInfo_SizeGetter<T>::alignment, murmurhash(name.m_buffer) };
		}

		template<size_t LEN>
		constexpr TypeInfo<LEN> getTypeInfo(const StringBuffer<LEN> &name, size_t size, size_t alignment)
		{
			return TypeInfo<LEN>{name.m_buffer, size, alignment, murmurhash(name.m_buffer) };
		}

		template<typename T, size_t LEN>
		constexpr TypeInfo<LEN> getTypeInfoParent(const char(&name)[LEN], const TypeInfoBase* parent)
		{
			return TypeInfo<LEN>{name, sizeof(T), __alignof(T), murmurhash(name), parent };
		}

		template<typename T>
		struct GetTypeInfo;

		template<typename Parent, typename Child>
		struct ParentGetter
		{
			static_assert(std::is_base_of<Parent, Child>::value, "");

			inline const TypeInfo* operator()() { return &::vx::type_info<Parent>(); }
		};

		template<typename T>
		struct GetTypeInfo<T*>
		{
			static const auto& get()
			{
				static const auto typeInfo{ getTypeInfo(concat(GetTypeInfo<T>::get().m_name, "*"), sizeof(T*), __alignof(T*)) };
				return typeInfo;
			}

			static constexpr auto get_constexpr()
			{
				return getTypeInfo(concat(GetTypeInfo<T>::get().m_name, "*"), sizeof(T*), __alignof(T*));
			}
		};

		template<typename T, typename F>
		struct GetTypeInfo<std::pair<T, F>>
		{
			static const auto& get()
			{
				static const auto typeInfo{ getTypeInfo(concat("std::pair<", GetTypeInfo<T>::get().m_name, ", ", GetTypeInfo<F>::get().m_name, ">"), sizeof(std::pair<T, F>), __alignof(std::pair<T, F>)) };
				return typeInfo;
			}

			static constexpr auto get_constexpr()
			{
				return getTypeInfo(concat("std::pair<", GetTypeInfo<T>::get().m_name, ", ", GetTypeInfo<F>::get().m_name, ">"), sizeof(std::pair<T, F>), __alignof(std::pair<T, F>));
			}
		};

		template<typename T>
		struct GetTypeInfo<std::unique_ptr<T>>
		{
			static const auto& get()
			{
				static const auto typeInfo{ getTypeInfo(concat("std::unique_ptr<", GetTypeInfo<T>::get().m_name, ">"), sizeof(std::unique_ptr<T>), __alignof(std::unique_ptr<T>)) };
				return typeInfo;
			}

			static constexpr auto get_constexpr()
			{
				return getTypeInfo(concat("std::unique_ptr<", GetTypeInfo<T>::get().m_name, ">"), sizeof(std::unique_ptr<T>), __alignof(std::unique_ptr<T>));
			}
		};

		namespace detail
		{
			template<u32 SIZE>
			struct GetTypeInfo<vx::StringArray<SIZE>>
			{
				static const auto& get()
				{
					static const auto typeInfo
					{
						get_constexpr()
					};
					return typeInfo;
				}

				constexpr static auto get_constexpr()
				{
					return getTypeInfo(concat("vx::StringArray<", IntToString<SIZE>::get().data, ">"),
						sizeof(vx::StringArray<SIZE>),
						__alignof(vx::StringArray<SIZE>));
				}
			};
		}
	}

	template<typename T>
	constexpr TypeInfoBase type_info()
	{
		return detail::GetTypeInfo<T>::get();
	}

	template<typename T>
	constexpr TypeInfoBase type_info_constexpr()
	{
		return detail::GetTypeInfo<T>::get_constexpr();
	}

	template<typename T>
	TypeInfoBase type_info(const T&)
	{
		return detail::GetTypeInfo<T>::get();
	}
}

VX_TYPEINFO_GENERATOR(u8)
VX_TYPEINFO_GENERATOR(u16)
VX_TYPEINFO_GENERATOR(u32)
VX_TYPEINFO_GENERATOR(u64)
VX_TYPEINFO_GENERATOR(s8)
VX_TYPEINFO_GENERATOR(s16)
VX_TYPEINFO_GENERATOR(s32)
VX_TYPEINFO_GENERATOR(s64)

VX_TYPEINFO_GENERATOR(f32)
VX_TYPEINFO_GENERATOR(f64)

VX_TYPEINFO_GENERATOR(vx::float2)
VX_TYPEINFO_GENERATOR(vx::float2a)
VX_TYPEINFO_GENERATOR(vx::float3)
VX_TYPEINFO_GENERATOR(vx::float4)
VX_TYPEINFO_GENERATOR(vx::float4a)

VX_TYPEINFO_GENERATOR(vx::uint2)
VX_TYPEINFO_GENERATOR(vx::uint2a)
VX_TYPEINFO_GENERATOR(vx::uint3)
VX_TYPEINFO_GENERATOR(vx::uint4)
VX_TYPEINFO_GENERATOR(vx::uint4a)

VX_TYPEINFO_GENERATOR(vx::int2)
VX_TYPEINFO_GENERATOR(vx::int2a)
VX_TYPEINFO_GENERATOR(vx::int3)
VX_TYPEINFO_GENERATOR(vx::int4)
VX_TYPEINFO_GENERATOR(vx::int4a)

VX_TYPEINFO_GENERATOR(vx::StringID)