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

		auto offset = Vector3Rotate(direction, m_qRotation);
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
		auto viewDir = Vector3Rotate(lookAt, m_qRotation);
		auto upDir = Vector3Rotate(up, m_qRotation);

		viewMatrix = vx::MatrixLookToRH(m_position, viewDir, upDir);
	}
}