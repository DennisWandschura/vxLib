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

#include <vxLib/TextPreprocessor.h>
#include <fstream>
#include <memory>

namespace TextPreprocessorCpp
{
	const char* findPreprocessorCommand(const char* p)
	{
		while (true)
		{
			auto c = p[0];

			if (c == '#')
				break;

			if (c == '\0')
			{
				p = nullptr;
				break;
			}

			++p;
		}

		return p;
	}

	std::string readFile(const char* filename)
	{
		std::string result;

		std::ifstream inFile(filename);
		if (inFile.is_open())
		{
			inFile.seekg(0, std::fstream::end);
			std::size_t size = inFile.tellg();
			inFile.seekg(0, std::fstream::beg);

			auto fileData = std::make_unique<char[]>(size + 1);
			inFile.read(fileData.get(), size);

			result.clear();
			result.reserve(size);
			result.assign(fileData.get());
		}

		return result;
	}

	const char* getCommandEnd(const char* p)
	{
		while (true)
		{
			if (p[0] == ' ' || p[0] == '\n')
				break;

			++p;
		}

		return p;
	}

	const char* getChar(const char* p, char c)
	{
		while (true)
		{
			if (p[0] == c)
				break;

			++p;
		}

		return p;
	}

	const char* getEndif(const char* currentPtr)
	{
		int layer = 0;
		while (true)
		{
			auto nextCommand = findPreprocessorCommand(currentPtr);
			auto nextCmdEnd = getCommandEnd(nextCommand);

			currentPtr = nextCmdEnd;

			std::string cmd;
			cmd.assign(nextCommand, nextCmdEnd);

			if (strcmp(cmd.c_str(), "#if") == 0 ||
				strcmp(cmd.c_str(), "#ifdef") == 0 ||
				strcmp(cmd.c_str(), "#ifndef") == 0)
			{
				++layer;
			}

			if (strcmp(cmd.c_str(), "#endif") == 0)
			{
				if (layer == 0)
					break;

				--layer;
			}
		}

		return currentPtr;
	}

	const char* getEndifOrElse(const char* p, bool *endif)
	{
		*endif = true;
		int layer = 0;
		while (true)
		{
			auto nextCommand = findPreprocessorCommand(p);
			auto nextCmdEnd = getCommandEnd(nextCommand);

			p = nextCmdEnd;

			std::string cmd;
			cmd.assign(nextCommand, nextCmdEnd);

			if (strcmp(cmd.c_str(), "#if") == 0 ||
				strcmp(cmd.c_str(), "#ifdef") == 0 ||
				strcmp(cmd.c_str(), "#ifndef") == 0)
			{
				++layer;
			}

			if (strcmp(cmd.c_str(), "#else") == 0)
			{
				if (layer == 0)
				{
					p = nextCommand;
					*endif = false;
					break;
				}
			}

			if (strcmp(cmd.c_str(), "#endif") == 0)
			{
				if (layer == 0)
				{
					p = nextCommand;
					break;
				}

				--layer;
			}
		}

		return p;
	}

	const char* handleCommandInclude
		(
			std::string* text,
			const char* cmdStart,
			const char* cmdEnd,
			const std::map<vx::StringID, std::string> &includeFiles,
			std::map<vx::StringID, int>* alreadyIncludedFiles
			)
	{
		auto result = cmdEnd;

		auto valueStart = getChar(cmdEnd, '"') + 1;
		auto valueEnd = getChar(valueStart, '"');

		std::string value;
		value.assign(valueStart, valueEnd);
		auto sid = vx::make_sid(value.c_str());

		bool changedText = false;
		auto alreadyIncludedIter = alreadyIncludedFiles->find(sid);
		if (alreadyIncludedIter == alreadyIncludedFiles->end())
		{
			auto it = includeFiles.find(sid);
			if (it != includeFiles.end())
			{
				auto textBegin = text->c_str();
				auto textEnd = text->c_str() + text->size();
				auto offsetToInclude = cmdStart - textBegin;
				auto offsetAfterInclude = valueEnd - textBegin + 1;

				std::string newText;
				newText.assign(textBegin, textBegin + offsetToInclude);

				newText.append(it->second.c_str());

				auto offsetNew = newText.size();
				newText.append(textBegin + offsetAfterInclude, textEnd);

				//	printf("%s\n", newText.c_str());

				changedText = true;

				result = newText.c_str() + offsetToInclude;
				text->swap(newText);

				alreadyIncludedFiles->insert(std::make_pair(sid, 1));
			}
			else
			{
				//printf("could not find file to include: %s\n", value.c_str());
			}
		}

		if (!changedText)
		{
			// already included
			auto size = valueEnd - cmdStart + 1;
			memset((void*)cmdStart, ' ', size);
			result = valueEnd;
		}

		return result;
	}

	const char* handleCommandIfdef
		(
			std::string* text,
			const char* cmdStart,
			const char* cmdEnd,
			const std::map<vx::StringID, int> &localDefines,
			const std::map<vx::StringID, int> &globalDefines,
			bool b
			)
	{
		auto result = cmdEnd;

		auto valueStart = getChar(cmdEnd, ' ') + 1;
		auto valueEnd = getChar(valueStart, '\n');

		std::string value;
		value.assign(valueStart, valueEnd);
		auto sid = vx::make_sid(value.c_str());

		bool foundDefine = b;

		auto it = localDefines.find(sid);
		if (it == localDefines.end())
		{
			auto globalIter = globalDefines.find(sid);
			if (globalIter == globalDefines.end())
			{
				foundDefine = !foundDefine;
			}
		}

		bool isEndif = true;
		auto other = getEndifOrElse(valueEnd, &isEndif);
		//auto bb = foundDefine;

		if (foundDefine)
		{
			// zero #ifdef line
			auto size = valueEnd - cmdStart;
			memset((void*)cmdStart, ' ', size);

			if (isEndif)
			{
				auto endifEnd = getChar(other, '\n');
				memset((void*)other, ' ', endifEnd - other);

				//result = endifEnd;
			}
			else
			{
				auto endif = getEndif(other);
				auto diff = endif - other;

				memset((void*)other, ' ', diff);

				//result = endif;
			}
		}
		else
		{
			if (isEndif)
			{
				auto endifEnd = getChar(other, '\n');
				auto size = endifEnd - cmdStart;
				memset((void*)cmdStart, ' ', size);

				//result = other;

			}
			else
			{
				auto elseEnd = getChar(other, '\n');
				auto endifStart = getEndifOrElse(elseEnd, &isEndif);
				auto endifEnd = getChar(endifStart, '\n');

				auto size = elseEnd - cmdStart;
				memset((void*)cmdStart, ' ', size);

				memset((void*)endifStart, ' ', endifEnd - endifStart);

				//result = endifEnd;
			}
		}

		return result;
	}

	const char* handlePreprocessorCommand
		(
			std::string* text,
			const char* cmdStart,
			const char* cmdEnd,
			const std::string &cmd, const std::map<vx::StringID, std::string> &includeFiles,
			std::map<vx::StringID, int>* alreadyIncludedFiles,
			std::map<vx::StringID, int>* localDefines,
			const std::map<vx::StringID, int> &globalDefines
			)
	{
		auto result = cmdEnd;

		if (strcmp(cmd.c_str(), "#include") == 0)
		{
			result = handleCommandInclude(text, cmdStart, cmdEnd, includeFiles, alreadyIncludedFiles);
		}
		else if (strcmp(cmd.c_str(), "#ifndef") == 0)
		{
			result = handleCommandIfdef(text, cmdStart, cmdEnd, *localDefines, globalDefines, false);
		}
		else if (strcmp(cmd.c_str(), "#ifdef") == 0)
		{
			result = handleCommandIfdef(text, cmdStart, cmdEnd, *localDefines, globalDefines, true);
		}
		else if (strcmp(cmd.c_str(), "#define") == 0)
		{
			auto valueStart = getChar(cmdEnd, ' ') + 1;
			auto valueEnd = getChar(valueStart, '\n');

			std::string value;
			value.assign(valueStart, valueEnd);
			auto sid = vx::make_sid(value.c_str());

			auto it = localDefines->find(sid);
			if (it == localDefines->end())
				localDefines->insert(std::make_pair(sid, 1));
		}
		else
		{
			//printf("unknown command: '%s'\n", cmd.c_str());
		}

		return result;
	}

	const char* findCustomValue(const char* p)
	{
		while (true)
		{
			if (p[0] == '\0')
				return nullptr;

			if (p[0] == '$')
				break;

			++p;
		}

		return p;
	}
}

namespace vx
{
	void TextPreprocessor::processText(std::string* text)
	{
		std::map<vx::StringID, int> alreadyIncludedFiles;
		std::map<vx::StringID, int> localDefines;

		auto current = text->c_str();

		while (true)
		{
			auto command = TextPreprocessorCpp::findPreprocessorCommand(current);
			if (command)
			{
				auto cmdEnd = TextPreprocessorCpp::getCommandEnd(command);

				std::string cmd;
				cmd.assign(command, cmdEnd);

				current = TextPreprocessorCpp::handlePreprocessorCommand(text, command, cmdEnd, cmd, m_includeFiles, &alreadyIncludedFiles, &localDefines, m_globalDefines);
			}
			else
				break;
		}

		current = text->c_str();
		while (true)
		{
			auto begin = TextPreprocessorCpp::findCustomValue(current);
			if (begin)
			{
				++begin;
				auto customValueEnd = TextPreprocessorCpp::getChar(begin, '$');

				std::string customValueKey;
				customValueKey.assign(begin, customValueEnd);

				auto sid = vx::make_sid(customValueKey.c_str());

				auto it = m_customValues.find(sid);
				if (it != m_customValues.end())
				{
					--begin;
					auto size = customValueEnd - begin + 1;

					auto customValue = it->second;

					char buffer[64];
					u32 len = 0;
					if (customValue.type == CustomValueData::Int)
					{
						len = sprintf(buffer, "%i", customValue.i);
					}
					else if (customValue.type == CustomValueData::Uint)
					{
						len = sprintf(buffer, "%u", customValue.u);
					}
					else
					{
						len = sprintf(buffer, "%f", customValue.f);
					}

					if (len <= size)
					{
						memcpy((void*)begin, buffer, len);
						begin += len;
						size -= len;

						memset((void**)begin, ' ', size);
					}
					else
					{
						auto textSize = text->size();
						auto textBegin = text->c_str();
						auto textEnd = textBegin + textSize;

						std::string nextText;
						nextText.assign(textBegin, begin);

						nextText.append(buffer, buffer + len);
						nextText.append(customValueEnd + 1, textEnd);

						text->swap(nextText);
					}
				}
				//

				current = customValueEnd + 1;
			}
			else
				break;
		}
	}

	void TextPreprocessor::setDefine(const char* id)
	{
		auto sid = vx::make_sid(id);
		auto it = m_globalDefines.find(sid);
		if (it == m_globalDefines.end())
		{
			m_globalDefines.insert(std::make_pair(sid, 1));
		}
	}

	void TextPreprocessor::removeDefine(const char* id)
	{
		auto sid = vx::make_sid(id);
		auto it = m_globalDefines.find(sid);
		if (it != m_globalDefines.end())
		{
			m_globalDefines.erase(it);
		}
	}

	void TextPreprocessor::setCustomValue(const char* id, s32 value)
	{
		auto sid = vx::make_sid(id);
		auto it = m_customValues.find(sid);
		if (it == m_customValues.end())
		{
			CustomValueData data;
			data.i = value;
			data.type = CustomValueData::Int;

			m_customValues.insert(std::make_pair(sid, data));
		}
		else
		{
			it->second.i = value;
			it->second.type = CustomValueData::Int;
		}
	}

	void TextPreprocessor::setCustomValue(const char* id, u32 value)
	{
		auto sid = vx::make_sid(id);
		auto it = m_customValues.find(sid);
		if (it == m_customValues.end())
		{
			CustomValueData data;
			data.i = value;
			data.type = CustomValueData::Uint;

			m_customValues.insert(std::make_pair(sid, data));
		}
		else
		{
			it->second.u = value;
			it->second.type = CustomValueData::Uint;
		}
	}

	void TextPreprocessor::setCustomValue(const char* id, f32 value)
	{
		auto sid = vx::make_sid(id);
		auto it = m_customValues.find(sid);
		if (it == m_customValues.end())
		{
			CustomValueData data;
			data.f = value;
			data.type = CustomValueData::Float;

			m_customValues.insert(std::make_pair(sid, data));
		}
		else
		{
			it->second.i = value;
			it->second.type = CustomValueData::Float;
		}
	}

	void TextPreprocessor::loadIncludeFile(const char* filename, const char* key)
	{
		auto sid = vx::make_sid(key);
		auto it = m_includeFiles.find(sid);
		if (it == m_includeFiles.end())
		{
			auto fileData = TextPreprocessorCpp::readFile(filename);
			m_includeFiles.insert(std::make_pair(std::move(sid), std::move(fileData)));
		}
	}

	std::string TextPreprocessor::preprocessFile(const char* filename)
	{
		auto fileData = TextPreprocessorCpp::readFile(filename);
		processText(&fileData);

		return fileData;
	}
}