/*
The MIT License(MIT)

Copyright(c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vxLib/Graphics/Camera.h>

namespace vx
{
	namespace Graphics
	{
		Camera::Camera()
			:m_position(_mm_setzero_ps()),
            m_qRotation(g_VXIdentityR3)
		{
		}

		void Camera::setPosition(f32 x, f32 y, f32 z)
		{
			__m128 position = { x, y, z, 0 };
			setPosition(position);
		}

		void VX_CALLCONV Camera::setPosition(const __m128 position)
		{
			m_position = position;
		}

		void VX_CALLCONV Camera::setRotation(const __m128 qRotation)
		{
			m_qRotation = qRotation;
		}

		void VX_CALLCONV Camera::rotate(const __m128 qRotation)
		{
			m_qRotation = _mm_add_ps(m_qRotation, qRotation);
		}

		void VX_CALLCONV Camera::move(const __m128 direction, f32 speed)
		{
			__m128 vSpeed = { speed, speed, speed, 0.0 };

			auto offset = quaternionRotation(direction, m_qRotation);

			offset = _mm_mul_ps(offset, vSpeed);
			m_position = _mm_add_ps(offset, m_position);
		}

		void Camera::move(f32 x, f32 y, f32 z)
		{
			__m128 offset = { x, y, z, 0.0f };
			m_position = _mm_add_ps(m_position, offset);
		}

		void Camera::getViewMatrixRH(vx::mat4 *viewMatrix) const
		{
			const __m128 lookAt = { 0, 0, -1, 0 };
			const __m128 up = { 0, 1, 0, 0 };

			auto viewDir = quaternionRotation(lookAt, m_qRotation);
			auto upDir = quaternionRotation(up, m_qRotation);

			*viewMatrix = vx::MatrixLookToRH(m_position, viewDir, upDir);
		}

		void Camera::getViewMatrixLH(vx::mat4 *viewMatrix) const
		{
			const __m128 lookAt = { 0, 0, 1, 0 };
			const __m128 up = { 0, 1, 0, 0 };

			auto viewDir = quaternionRotation(lookAt, m_qRotation);
			auto upDir = quaternionRotation(up, m_qRotation);

			*viewMatrix = vx::MatrixLookToLH(m_position, viewDir, upDir);
		}
	}
}
