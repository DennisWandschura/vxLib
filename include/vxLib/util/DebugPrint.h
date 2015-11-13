#pragma once
#ifndef __VX_DEBUG_PRINT_H
#define __VX_DEBUG_PRINT_H
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

#include <vxLib/types.h>
#include <cstdio>

namespace vx
{
	namespace debugPrint
	{
		extern u16 g_filter;
		extern u32 g_verbosity;
		extern void* g_hConsole;
	};

	static void activateChannel(u8 channel)
	{
		debugPrint::g_filter |= channel;
	}

	static void deactivateChannel(u8 channel)
	{
		debugPrint::g_filter ^= channel;
	}

	static bool checkChannel(u8 channel)
	{
		return ((debugPrint::g_filter & channel) != 0);
	}

	extern void setConsoleFormat(u8 channel);
	extern void debugPrintF(const char *format, va_list argList);
	extern void debugFPrintF(FILE *stream, const char *format, va_list argList);
	extern void verbosePrintF(u32 verbosity, const char *format, ...);
	extern void verboseFPrintF(u32 verbosity, FILE *stream, const char *format, ...);
	extern void verboseChannelPrintF(u32 verbosity, u8 channel, const char *format, ...);
	extern void verboseChannelFPrintF(u32 verbosity, u8 channel, FILE *stream, const char *format, ...);

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