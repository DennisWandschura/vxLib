#ifndef __VX_GL_DEBUG_H
#define __VX_GL_DEBUG_H
#pragma once

#include <vxLib\types.h>
#include <iostream>

namespace vx
{
	namespace gl
	{
		using DebugHighSeverityCallback = void(__stdcall*)();

		class Debug
		{
			static void errorCallback(U32 source, U32 type, U32 id, U32 severity, I32 length, const char *message, const void *userParam);

			static void printStack();

			/// The stream to where messages are directed
			static std::ostream *s_pOuts;

			static void *s_pUserData;

			static DebugHighSeverityCallback s_highSeverityCallback;

		public:
			static bool initialize(std::ostream *pOutStream = nullptr);

			static void __stdcall enableCallback(bool enable);

			static void enableLowSeverityMessages(bool enabled);

			static void setHighSeverityCallback(DebugHighSeverityCallback callback);
		};
	}
}
#endif