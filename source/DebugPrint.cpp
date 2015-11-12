/*
The MIT License(MIT)

Copyright(c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <vxLib\util\DebugPrint.h>

namespace vx
{
	u32 debugPrint::g_verbosity = 0;
	u16 debugPrint::g_filter = 0;
	HANDLE debugPrint::g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	void debugPrintF(const char *format, va_list argList)
	{
		const u32 max = 1023;
		static char sBuffer[max + 1];

		vsnprintf_s(sBuffer, max, format, argList);
		sBuffer[max] = '\0';

		puts(sBuffer);
	}

	void debugFPrintF(FILE *stream, const char *format, va_list argList)
	{
		const u32 max = 1023;
		static char sBuffer[max + 2];

		int size = vsnprintf_s(sBuffer, max, format, argList);
		sBuffer[size] = '\n';
		sBuffer[size + 1] = '\0';

		fputs(sBuffer, stream);
	}

	void verbosePrintF(u32 verbosity, const char *format, ...)
	{
		if (vx::debugPrint::g_verbosity >= verbosity)
		{
			va_list argList;
			va_start(argList, format);

			vx::debugPrintF(format, argList);

			va_end(argList);
		}
	}

	void verboseFPrintF(u32 verbosity, FILE *stream, const char *format, ...)
	{
		if (vx::debugPrint::g_verbosity >= verbosity)
		{
			va_list argList;
			va_start(argList, format);

			vx::debugFPrintF(stream, format, argList);

			va_end(argList);
		}
	}

	void setConsoleFormat(u8 channel)
	{
		switch (channel)
		{
		case(CHANNEL_ONE) :
			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 9);
			break;
		case(CHANNEL_TWO) :
			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 10);
			break;
		case(CHANNEL_THREE) :
			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 11);
			break;
		case(CHANNEL_FOUR) :
			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 12);
			break;
		case(CHANNEL_FIVE) :
			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 13);
			break;
		case(CHANNEL_SIX) :
			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 14);
			break;
		case(CHANNEL_SEVEN) :
			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 15);
			break;
		case(CHANNEL_ERROR) :
			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 15 | BACKGROUND_RED | BACKGROUND_INTENSITY);
			break;
		}
	}

	void verboseChannelPrintF(u32 verbosity, u8 channel, const char *format, ...)
	{
		if (vx::checkChannel(channel) && vx::debugPrint::g_verbosity >= verbosity)
		{
			setConsoleFormat(channel);

			va_list argList;
			va_start(argList, format);

			vx::debugPrintF(format, argList);

			va_end(argList);

			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 7);
		}
	}

	void verboseChannelFPrintF(u32 verbosity, u8 channel, FILE *stream, const char *format, ...)
	{
		if (vx::checkChannel(channel) && vx::debugPrint::g_verbosity >= verbosity)
		{
			setConsoleFormat(channel);

			va_list argList;
			va_start(argList, format);

			vx::debugFPrintF(stream, format, argList);

			va_end(argList);

			SetConsoleTextAttribute(vx::debugPrint::g_hConsole, 7);
		}
	}
}