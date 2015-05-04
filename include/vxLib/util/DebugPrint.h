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
#ifndef __VX_DEBUG_PRINT_H
#define __VX_DEBUG_PRINT_H
#pragma once

#include <vxLib\types.h>
#include <cstdio>
#include <Windows.h>

namespace vx
{
	namespace DebugPrint
	{
		extern U16 g_filter;
		extern U32 g_verbosity;
		extern HANDLE g_hConsole;
	};

	static void activateChannel(U8 channel)
	{
		DebugPrint::g_filter |= channel;
	}

	static void deactivateChannel(U8 channel)
	{
		DebugPrint::g_filter ^= channel;
	}

	static bool checkChannel(U8 channel)
	{
		return ((DebugPrint::g_filter & channel) != 0);
	}

	extern void setConsoleFormat(U8 channel);
	extern inline void debugPrintF(const char *format, va_list argList);
	extern inline void debugFPrintF(FILE *stream, const char *format, va_list argList);
	extern inline void verbosePrintF(U32 verbosity, const char *format, ...);
	extern inline void verboseFPrintF(U32 verbosity, FILE *stream, const char *format, ...);
	extern void verboseChannelPrintF(U32 verbosity, U8 channel, const char *format, ...);
	extern void verboseChannelFPrintF(U32 verbosity, U8 channel, FILE *stream, const char *format, ...);

#define CHANNEL_ONE 0x0001
#define CHANNEL_TWO 0x0002
#define CHANNEL_THREE 0x0004
#define CHANNEL_FOUR 0x0008
#define CHANNEL_FIVE 0x0010
#define CHANNEL_SIX 0x0020
#define CHANNEL_SEVEN 0x0040
#define CHANNEL_ERROR 0x0080
}
#endif