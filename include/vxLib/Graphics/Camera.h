#ifndef __VX_CAMERA_H
#define __VX_CAMERA_H
#pragma once

#include <vxLib/math/matrix.h>

namespace vx
{
	class Camera
	{
		__m128 m_position;
		__m128 m_qRotation;

	public:
		Camera();

		void setPosition(F32 x, F32 y, F32 z);
		void setPosition(const vx::float3 &position);
		void VX_CALLCONV setPosition(const __m128 position);
		void VX_CALLCONV setRotation(const __m128 qRotation);

		void VX_CALLCONV rotate(const __m128 qRotation);
		void VX_CALLCONV move(const __m128 direction, const F32 &speed);
		void move(F32 x, F32 y, F32 z);

		void getViewMatrix(vx::mat4 &viewMatrix) const;
		const __m128 VX_CALLCONV getPosition() const { return m_position; }
		const __m128 VX_CALLCONV getRotation() const { return m_qRotation; }
	};
}
#endif