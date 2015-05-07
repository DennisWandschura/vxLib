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
	Camera::Camera()
		:m_position(_mm_setzero_ps()),
		m_qRotation(g_VXIdentityR3)
	{
	}

	void Camera::setPosition(F32 x, F32 y, F32 z)
	{
		auto vX = _mm_load_ss(&x);
		auto vY = _mm_load_ss(&y);

		// x y 0 0
		m_position = _mm_unpacklo_ps(vX, vY);

		auto vZ = _mm_load_ss(&z);
		// x y z 0
		m_position = _mm_movelh_ps(m_position, vZ);
	}

	void Camera::setPosition(const vx::float3 &position)
	{
		m_position = vx::loadFloat(position);
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

	void VX_CALLCONV Camera::move(const __m128 direction, const F32 &speed)
	{
		__m128 vSpeed = _mm_load_ss(&speed);
		vSpeed = VX_PERMUTE_PS(vSpeed, _MM_SHUFFLE(3, 0, 0, 0));

		//auto q = DirectX::XMQuaternionRotationRollPitchYawFromVector(m_rotation);

		auto offset = quaternionRotation(direction, m_qRotation);
		m_position = _mm_fmadd_ps(offset, vSpeed, m_position);

		//offset = _mm_mul_ps(offset, vSpeed);
		//m_position = _mm_add_ps(m_position, offset);
	}

	void Camera::move(F32 x, F32 y, F32 z)
	{
		__m128 offset = { x, y, z, 0.0f };
		m_position = _mm_add_ps(m_position, offset);
	}

	void Camera::getViewMatrix(vx::mat4 &viewMatrix) const
	{
		const __m128 lookAt = { 0, 0, -1, 0 };
		const __m128 up = { 0, 1, 0, 0 };
		// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.

		// Create the rotation matrix from the yaw, pitch, and roll values.
		//auto q = DirectX::XMQuaternionRotationRollPitchYawFromVector(m_rotation);
		//auto q = vx::quaternionRotationRollPitchYawFromVector(m_rotation);
		//auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(m_rotation);

		// Setup where the camera is looking by default.


		//m_direction = DirectX::XMVector3Transform(lookAt, rotationMatrix);
		//up = DirectX::XMVector3Transform(up, rotationMatrix);
		auto viewDir = quaternionRotation(lookAt, m_qRotation);
		auto upDir = quaternionRotation(up, m_qRotation);

		viewMatrix = vx::MatrixLookToRH(m_position, viewDir, upDir);
	}
}