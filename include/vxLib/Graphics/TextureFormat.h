#include <vxlib/types.h>

namespace vx
{
	namespace graphics
	{
		enum class TextureFormat : u8
		{
			Unkown,
			RED,
			BG,
			BGR,
			BGRA,
			RG,
			RGB,
			RGBA,
			SRGBA,
			DXT1,
			DXT3,
			DXT5,
			BC7_UNORM_SRGB,
			BC7_UNORM,
			BC6H_UF16,
			BC6H_SF16
		};
	}
}