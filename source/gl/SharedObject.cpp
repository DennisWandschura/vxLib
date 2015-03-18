#include <vxLib/gl/SharedObject.h>

namespace vx
{
	namespace gl
	{
		namespace detail
		{
			SharedObjectBase::SharedObjectBase(const SharedObjectBase &rhs) noexcept
				: m_pRefCount(rhs.m_pRefCount)
			{
				incRefCount();
			}

			SharedObjectBase::SharedObjectBase(SharedObjectBase &&rhs) noexcept
				: m_pRefCount(rhs.m_pRefCount)
			{
				rhs.m_pRefCount = nullptr;
			}

			SharedObjectBase& SharedObjectBase::operator = (const SharedObjectBase &rhs) noexcept
			{
				if (this != &rhs)
				{
					m_pRefCount = rhs.m_pRefCount;
					incRefCount();
				}
				return *this;
			}

				SharedObjectBase& SharedObjectBase::operator = (SharedObjectBase &&rhs) noexcept
			{
				if (this != &rhs)
				{
					m_pRefCount = rhs.m_pRefCount;
					rhs.m_pRefCount = nullptr;
				}
				return *this;
			}

			void SharedObjectBase::create(U32 id)
			{
				if (!m_pRefCount)
				{
					//	U32 id = T::create();
					U32 packedId = id & s_idMask;
					VX_ASSERT(id == packedId);

					m_pRefCount = new U32(1);
					auto maskedPtr = maskPtr();
					VX_ASSERT(maskedPtr == (size_t)m_pRefCount);

					m_pRefCount = (U32*)(maskedPtr | (static_cast<size_t>(packedId) << 48));
				}
			}

			U32 SharedObjectBase::destroy()
			{
				if (m_pRefCount)
				{
					auto ptr = getPtr();

					if (*ptr == 1)
					{
						auto id = getId();

						delete(ptr);
						m_pRefCount = nullptr;
						return id;
					}
					else
					{
						--(*ptr);
					}
					return 0;
				}

				return 0;
			}

			void SharedObjectBase::incRefCount() noexcept
			{
				if (m_pRefCount)
				{
					auto ptr = getPtr();
					++(*ptr);
				}
			}

				U32 SharedObjectBase::getId() const noexcept
			{
				return static_cast<U32>((size_t)m_pRefCount >> 48) & s_idMask;
			}

				bool SharedObjectBase::valid() const noexcept
			{
				return (m_pRefCount != nullptr);
			}
		}
	}
}