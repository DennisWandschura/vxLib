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
		:m_position(_mm256_setzero_pd()),
		m_qRotation({0, 0, 0, 1})
	{
	}

	void Camera::setPosition(f64 x, f64 y, f64 z)
	{
		__m256d position;
		position.m256d_f64[0] = x;
		position.m256d_f64[1] = y;
		position.m256d_f64[2] = z;
		position.m256d_f64[3] = 0;

		setPosition(position);
	}

	void VX_CALLCONV Camera::setPosition(const __m256d position)
	{
		m_position = position;
	}

	void VX_CALLCONV Camera::setRotation(const __m256d qRotation)
	{
		m_qRotation = qRotation;
	}

	void VX_CALLCONV Camera::rotate(const __m256d qRotation)
	{
		m_qRotation = _mm256_add_pd(m_qRotation, qRotation);
	}

	void VX_CALLCONV Camera::move(const __m256d direction, f64 speed)
	{
		__m256d vSpeed = {speed, speed, speed, 0.0};

		auto offset = quaternionRotation(direction, m_qRotation);

		m_position = _mm256_fmadd_pd(offset, vSpeed, m_position);
		//m_position = _mm256_add_pd(_mm256_mul_pd(offset, vSpeed), m_position);
	}

	void Camera::move(f64 x, f64 y, f64 z)
	{
		__m256d offset = { x, y, z, 0.0f };
		m_position = _mm256_add_pd(m_position, offset);
	}

	void Camera::getViewMatrix(vx::mat4d *viewMatrix) const
	{
		const __m256d lookAt = { 0, 0, -1, 0 };
		const __m256d up = { 0, 1, 0, 0 };
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

		*viewMatrix = vx::MatrixLookToRH(m_position, viewDir, upDir);
	}
}