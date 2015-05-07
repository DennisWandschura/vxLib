/*
The MIT License (MIT)

Copyright (c) 2015 Dennis Wandschura

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

		void setPosition(f32 x, f32 y, f32 z);
		void setPosition(const vx::float3 &position);
		void VX_CALLCONV setPosition(const __m128 position);
		void VX_CALLCONV setRotation(const __m128 qRotation);

		void VX_CALLCONV rotate(const __m128 qRotation);
		void VX_CALLCONV move(const __m128 direction, const f32 &speed);
		void move(f32 x, f32 y, f32 z);

		void getViewMatrix(vx::mat4 &viewMatrix) const;
		const __m128 VX_CALLCONV getPosition() const { return m_position; }
		const __m128 VX_CALLCONV getRotation() const { return m_qRotation; }
	};
}
#endif