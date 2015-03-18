#ifndef __VX_GL_SHAREDOBJECT_H
#define __VX_GL_SHAREDOBJECT_H
#pragma once

#include <vxLib\gl\Base.h>

namespace vx
{
	namespace gl
	{
		namespace detail
		{
			class SharedObjectBase
			{
				static const U32 s_idMask = 0x0000FFFFu;
				static const size_t s_ptrMask = 0x0000FFFFFFFFFFFFu;

			protected:
				// packed id in last 16 bytes,
				U32 *m_pRefCount{nullptr};

				inline size_t maskPtr() const noexcept
				{
					return ((size_t)m_pRefCount & s_ptrMask);
				}

					inline U32* getPtr() const noexcept
				{
					return reinterpret_cast<U32*>((size_t)m_pRefCount & s_ptrMask);
				}

				void create(U32 id);

				U32 destroy();

				void incRefCount() noexcept;

			public:
				SharedObjectBase(){}
				SharedObjectBase(const SharedObjectBase &rhs) noexcept;
				SharedObjectBase(SharedObjectBase &&rhs) noexcept;

				SharedObjectBase& operator=(const SharedObjectBase &rhs) noexcept;
				SharedObjectBase& operator=(SharedObjectBase &&rhs) noexcept;

				U32 getId() const noexcept;

				bool valid() const noexcept;
			};
		}

		template<class T>
		class SharedObject : public detail::SharedObjectBase
		{
			typedef  detail::SharedObjectBase MyBase;
		protected:
			~SharedObject()
			{
				if (m_pRefCount)
					static_cast<T*>(this)->destroy();
			}
		public:
			SharedObject() noexcept : MyBase(){}

			SharedObject(const SharedObject &rhs) noexcept
				: MyBase(rhs){}

			SharedObject(SharedObject &&rhs) noexcept
				: MyBase(std::move(rhs)){}

			SharedObject& operator=(const SharedObject &rhs) noexcept
			{
				MyBase::operator=(rhs);
				return *this;
			}

			SharedObject& operator=(SharedObject &&rhs) noexcept
			{
				MyBase::operator=(std::move(rhs));
				return *this;
			}
		};
	}
}
#endif