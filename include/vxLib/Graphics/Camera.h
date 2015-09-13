#pragma once
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

#include <vxLib/math/matrix.h>

namespace vx
{
	class Camera
	{
		__m256d m_position;
		__m256d m_qRotation;

	public:
		Camera();

		void setPosition(f64 x, f64 y, f64 z);
		void VX_CALLCONV setPosition(const __m256d position);
		void VX_CALLCONV setRotation(const __m256d qRotation);

		void VX_CALLCONV rotate(const __m256d qRotation);
		void VX_CALLCONV move(const __m256d direction, f64 speed);
		void move(f64 x, f64 y, f64 z);

		void getViewMatrixRH(vx::mat4d *viewMatrix) const;
		void getViewMatrixLH(vx::mat4d *viewMatrix) const;
		const __m256d VX_CALLCONV getPosition() const { return m_position; }
		const __m256d VX_CALLCONV getRotation() const { return m_qRotation; }
	};
}