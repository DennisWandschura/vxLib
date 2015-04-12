#ifndef __VX_GL_RENDERCONTEXT_H
#define __VX_GL_RENDERCONTEXT_H
#pragma once

#include <vxLib\math\matrix.h>
#include <Windows.h>
#include <memory>

namespace vx
{
	class Window;
	
	namespace gl
	{
		struct OpenGLDescription
		{
			HWND hwnd;
			vx::uint2 resolution;
			F32 fovRad; 
			F32 nearZ;
			F32 farZ;
			U8 majVersion;
			U8 minVersion;
			bool bVsync;
			bool bDebugMode;

			static OpenGLDescription create(const Window &window, const vx::uint2 &resolution, F32 fovRad, F32 nearZ, F32 farZ, U8 majVersion, U8 minVersion, bool bVsync, bool bDebugMode);
			static OpenGLDescription create(HWND hwnd, const vx::uint2 &resolution, F32 fovRad, F32 nearZ, F32 farZ, U8 majVersion, U8 minVersion, bool bVsync, bool bDebugMode);
		};

		struct ContextDescription
		{
			HINSTANCE hInstance;
			LPCWSTR windowClass;
			OpenGLDescription glParams;

			static ContextDescription create(const Window &window, const vx::uint2 &resolution, F32 fovRad, F32 nearZ, F32 farZ, U8 majVersion, U8 minVersion, bool bVsync, bool bDebugMode);
		};

		class RenderContext
		{
			mat4 m_orthoMatrix;
			mat4 m_projectionMatrix;
			HDC m_pDeviceContext;
			HGLRC m_pRenderingContext;
			std::unique_ptr<I32[]> m_pContextAttribs;

			bool initializeExtensionsWithTempWindow(HINSTANCE hInstance, LPCWSTR windowClass);
			bool initializeOpenGl(const OpenGLDescription &params, const int *pContextAttribs);

			void setDefaultStates(const OpenGLDescription &params);

		public:
			RenderContext();
			~RenderContext();

			bool initialize(const ContextDescription &params);

			bool initializeExtensions(HWND hwnd);
			bool initializeOpenGl(const OpenGLDescription &desc);

			void shutdown(HWND hwnd);
			void shutdown(const Window &window);

			void swapBuffers();

			void makeCurrent();

			const mat4& VX_CALLCONV getProjectionMatrix() const
			{
				return m_projectionMatrix;
			}

			const mat4& VX_CALLCONV getOrthoMatrix() const
			{
				return m_orthoMatrix;
			}

			const HDC getDeviceContext() const
			{
				return m_pDeviceContext;
			}

			const HGLRC getRenderingContext() const
			{
				return m_pRenderingContext;
			}

			const I32* getContextAttributes() const { return m_pContextAttribs.get(); }
		};
	}
}
#endif