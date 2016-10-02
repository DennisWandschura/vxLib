#include <vxlib/Graphics/Surface.h>
#include <algorithm>

namespace vx
{
	namespace graphics
	{
		Surface::Surface()
			: m_dimension(0, 0, 0),
			m_size(0),
			m_pixels()
		{
		}

		Surface::Surface(Surface &&rhs)
			:m_dimension(rhs.m_dimension),
			m_size(rhs.m_size),
			m_pixels(std::move(rhs.m_pixels))
		{
			rhs.m_size = 0;
		}

		Surface::~Surface()
		{
		}

		Surface& Surface::operator = (Surface &&rhs)
		{
			if (this != &rhs)
			{
				std::swap(m_dimension, rhs.m_dimension);
				std::swap(m_size, rhs.m_size);
				std::swap(m_pixels, rhs.m_pixels);
			}
			return *this;
		}

		void Surface::create(const vx::uint3 &dimension, u32 imgsize, const vx::AllocatedBlock &pixels)
		{
			VX_ASSERT(dimension.x != 0);
			VX_ASSERT(dimension.y != 0);
			VX_ASSERT(dimension.z != 0);
			VX_ASSERT(imgsize != 0);
			VX_ASSERT(pixels.ptr);

			m_dimension = dimension;
			m_size = imgsize;
			m_pixels = std::move(pixels);
		}

		vx::AllocatedBlock Surface::release()
		{
			auto block = m_pixels;

			m_pixels.ptr = nullptr;
			m_pixels.size = 0;
			m_size = 0;

			return block;
		}
	}
}