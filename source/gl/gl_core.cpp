#include <vxLib/gl/gl.h>

namespace vx
{
	namespace gl
	{
		bool gl_load(HDC deviceContext)
		{
			/*glUniform1ui64NV = (PFNGLUNIFORM1UI64NVPROC)wglGetProcAddress("glUniform1ui64NV");
			if (!glUniform1ui64NV)
			return false;*/

			if (flextInit() == GL_FALSE)
				return false;

			if (wgl_LoadFunctions(deviceContext) == wgl_LOAD_FAILED)
				return false;

			return true;
		}
	}
}