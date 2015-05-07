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
#ifndef __VX_GL_BASE_H
#define __VX_GL_BASE_H
#pragma once

#include <vxLib/types.h>
#include <algorithm>

namespace vx
{
	namespace gl
	{
		enum class DataType : U8
		{
			Unsigned_Byte,
			Byte,
			Unsigned_Short,
			Short,
			Unsigned_Int,
			Int,
			Float
		};

		enum class Capabilities : U8
		{
			Blend,
			Color_Logic_Op,
			Cull_Face,
			Debug_Output,
			Debug_Output_Syncronous,
			Depth_Clamp,
			Depth_Test,
			Dither,
			Framebuffer_sRGB,
			Line_Smooth,
			Multisample,
			Polygon_Offset_Fill,
			Polygon_Offset_Line,
			Polygon_Offset_Point,
			Polygon_Smooth,
			Primitive_Restart,
			Primitive_Restart_Fixed_Index,
			Rasterizer_Discard,
			Sample_Alpha_To_Coverage,
			Sample_Alpha_To_One,
			Sample_Coverage,
			Sample_Shading,
			Sample_Mask,
			Scissor_Test,
			Stencil_Test,
			Texture_Cube_Map_Seamless,
			Program_Point_Size
		};

		static const U32 g_maxCapabilityCount = 27u;

		enum class ShaderProgramType : U8
		{
			VERTEX,
			GEOMETRY,
			COMPUTE,
			FRAGMENT,
			TESS_CONTROL,
			TESS_EVAL
		};

		enum class TextureFilter : U8
		{
			NEAREST,
			LINEAR,
			LINEAR_MIPMAP_LINEAR
		};

		enum class TextureWrapMode : U8
		{
			CLAMP_TO_EDGE,
			CLAMP_TO_BORDER,
			MIRRORED_REPEAT,
			REPEAT,
			MIRROR_CLAMP_TO_EDGE
		};

		struct DrawElementsIndirectCommand
		{
			U32  count;
			U32  instanceCount;
			U32  firstIndex;
			U32  baseVertex;
			U32  baseInstance;
		};

		typedef  struct {
			U32  count;
			U32  instanceCount;
			U32  first;
			U32  baseInstance;
		} DrawArraysIndirectCommand;

		namespace detail
		{
			U32 getCapability(Capabilities cap);
			U32 getDataType(DataType dataType);
			U32 getShaderProgramType(ShaderProgramType type);
			U32 getTextureFilter(TextureFilter filter);
			U32 getTextureWrapMode(TextureWrapMode wrapMode);
		}

		template<class T>
		class Base
		{
		protected:
			U32 m_id;

			inline Base() noexcept :m_id(0){}

			inline ~Base()
			{
				static_cast<T*>(this)->destroy();
			}

		public:
			Base(const Base&) = delete;

			Base(Base &&rhs) noexcept
				:m_id(rhs.m_id)
			{
				rhs.m_id = 0;
			}

			Base& operator=(const Base&) = delete;

			Base& operator = (Base &&rhs) noexcept
			{
				if (this != &rhs)
				{
					this->swap(rhs);
				}

				return *this;
			}

			void swap(Base &rhs) noexcept
			{
				std::swap(m_id, rhs.m_id);
			}

			bool isValid() const
			{
				return m_id != 0;
			}

			inline U32 getId() const noexcept 
			{
				return m_id;
			}
		};
	}
}
#endif