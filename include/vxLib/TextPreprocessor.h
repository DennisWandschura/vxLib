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

#include <vxLib/types.h>
#include <map>
#include <string>
#include <vxLib/StringID.h>


namespace vx
{
	namespace detail
	{
		struct StringIdCmp
		{
			bool operator()(const StringID &lhs, const StringID &rhs) const
			{
				return lhs.value < rhs.value;
			}
		};
	}

	class TextPreprocessor
	{
		struct CustomValueData
		{
			enum Type { Float, Int, Uint };
			union
			{
				f32 f;
				s32 i;
				u32 u;
			};
			Type type;
		};

		std::map<StringID, std::string, detail::StringIdCmp> m_includeFiles;
		std::map<StringID, int, detail::StringIdCmp> m_globalDefines;
		std::map<StringID, CustomValueData, detail::StringIdCmp> m_customValues;

		void processText(std::string* text);

	public:
		TextPreprocessor();
		~TextPreprocessor();

		void setDefine(const char* id);
		void removeDefine(const char* id);

		void setCustomValue(const char* id, s32 value);
		void setCustomValue(const char* id, u32 value);
		void setCustomValue(const char* id, f32 value);

		void loadIncludeFile(const char* filename, const char* key);

		std::string preprocessFile(const char* filename);
	};
}
