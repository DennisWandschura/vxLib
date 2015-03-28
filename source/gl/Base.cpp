#include <vxLib/gl/Base.h>
#include <vxLib/gl/gl.h>

namespace vx
{
	namespace gl
	{
		namespace detail
		{
			const U32 g_capabilities[27] = { GL_BLEND, GL_COLOR_LOGIC_OP, GL_CULL_FACE, GL_DEBUG_OUTPUT, GL_DEBUG_OUTPUT_SYNCHRONOUS, GL_DEPTH_CLAMP,
				GL_DEPTH_TEST, GL_DITHER, GL_FRAMEBUFFER_SRGB, GL_LINE_SMOOTH, GL_MULTISAMPLE, GL_POLYGON_OFFSET_FILL, GL_POLYGON_OFFSET_LINE,
				GL_POLYGON_OFFSET_POINT, GL_POLYGON_SMOOTH, GL_PRIMITIVE_RESTART, GL_PRIMITIVE_RESTART_FIXED_INDEX, GL_RASTERIZER_DISCARD,
				GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_ONE, GL_SAMPLE_COVERAGE, GL_SAMPLE_SHADING, GL_SAMPLE_MASK, GL_SCISSOR_TEST,
				GL_STENCIL_TEST, GL_TEXTURE_CUBE_MAP_SEAMLESS, GL_PROGRAM_POINT_SIZE };

			const U32 g_dataTypes[7] =
			{
				GL_UNSIGNED_BYTE,
				GL_BYTE,
				GL_UNSIGNED_SHORT,
				GL_SHORT,
				GL_UNSIGNED_INT,
				GL_INT,
				GL_FLOAT
			};

			const U32 g_shaderProgramTypes[6] =
			{
				GL_VERTEX_SHADER,
				GL_GEOMETRY_SHADER,
				GL_COMPUTE_SHADER,
				GL_FRAGMENT_SHADER,
				GL_TESS_CONTROL_SHADER,
				GL_TESS_EVALUATION_SHADER
			};

			const U32 g_textureFilters[3] =
			{
				GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR
			};

#ifdef _VX_GL45
			const U32 g_textureWrapModes[5] =
			{
				GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE
			};
#else
			const U32 g_textureWrapModes[4] =
			{
				GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT
			};
#endif

			U32 getCapability(Capabilities cap)
			{
				return detail::g_capabilities[(U32)cap];
			}

			U32 getDataType(DataType dataType)
			{
				return detail::g_dataTypes[(U8)dataType];
			}

			U32 getShaderProgramType(ShaderProgramType type)
			{
				return detail::g_shaderProgramTypes[(U32)type];
			}

			U32 getTextureFilter(TextureFilter filter)
			{
				return detail::g_textureFilters[(U32)filter];
			}

			U32 getTextureWrapMode(TextureWrapMode wrapMode)
			{
				return g_textureWrapModes[(U32)wrapMode];
			}
		}
	}
}