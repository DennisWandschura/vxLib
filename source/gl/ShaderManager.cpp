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

#include <vxLib/gl/ShaderManager.h>
#include <vxLib/gl/ProgramPipeline.h>
#include <vxLib/gl/ShaderProgram.h>
#include <fstream>
#include <Shlwapi.h>
#include <vxLib/gl/gl.h>
#include <vxLib/memory.h>
#include <vxLib/File/FileHandle.h>
#include <vxLib/Variant.h>

namespace vx
{
	namespace gl
	{
		enum ShaderParameterType{ Int, Uint, Float };

		struct ShaderParameter
		{
			union
			{
				u32 u;
				s32 s;
				f32 f;
			};

			ShaderParameterType type;

			explicit ShaderParameter(u32 v) :u(v), type(ShaderParameterType::Uint){}
			explicit ShaderParameter(s32 v) :s(v), type(ShaderParameterType::Int){}
			explicit ShaderParameter(f32 v) :f(v), type(ShaderParameterType::Float){}

			int operator()(char(&paramBuffer)[64]) const
			{
				if (type == ShaderParameterType::Int)
				{
					return sprintf(paramBuffer, "%d", s);
				}
				else if (type == ShaderParameterType::Float)
				{
					return sprintf(paramBuffer, "%f", f);
				}
				else
				{
					return sprintf(paramBuffer, "%u", u);
				}
			}
		};
	}
}

namespace ShaderManagerCpp
{
	const char* findParameter(const char* str)
	{
		while (true)
		{
			if (str[0] == '\0' || str[0] == '$')
				break;

			++str;
		}

		return str;
	}

	bool insertParameter(std::unique_ptr<char[]>* str, const vx::sorted_vector<vx::StringID, vx::gl::ShaderParameter> &params)
	{
		auto paramBegin = findParameter(str->get());
		auto sizeToParam = paramBegin - str->get();
		if (paramBegin[0] == '\0')
			return false;

		++paramBegin;

		auto paramEnd = findParameter(paramBegin);
		if (paramEnd[0] == '\0')
			return false;

		auto paramNameSize = paramEnd - paramBegin;
		char paramName[64] = {};
		strncpy(paramName, paramBegin, paramNameSize);

		auto it = params.find(vx::make_sid(paramName));
		if (it == params.end())
		{
			printf("could not find value for parameter '%s'\n", paramName);
			return false;
		}

		++paramEnd;

		auto sizeToEnd = strlen(paramEnd);

		char paramBuffer[64] = {};

		auto valueSize = (*it)(paramBuffer);

		auto bufferSize = sizeToParam + valueSize + sizeToEnd;

		auto newText = std::make_unique<char[]>(bufferSize + 1);
		strncpy(newText.get(), str->get(), sizeToParam);
		strncpy(newText.get() + sizeToParam, paramBuffer, valueSize);
		strncpy(newText.get() + sizeToParam + valueSize, paramEnd, sizeToEnd);

		str->swap(newText);

		return true;
	}

	bool loadFile(const char* str, std::unique_ptr<char[]> &buffer, u32 &size)
	{
		std::ifstream inFile(str);

		if (!inFile.is_open())
		{
			printf("could not open file %s\n", str);
			return false;
		}

		inFile.seekg(0, std::ios::end);
		size = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		buffer = vx::make_unique<char[]>(size);

		inFile.read(buffer.get(), size);

		return true;
	}

	std::unique_ptr<char[]> appendInclude(const std::unique_ptr<char[]>* programBuffer, u32* programSize, const std::string &include, const char* includeStart)
	{
		auto includeSize = include.size();

		auto newProgram = vx::make_unique<char[]>(*programSize + includeSize);
		
		u32 currentSize = 0;

		auto size = includeStart - programBuffer->get();

		memcpy(newProgram.get(), programBuffer->get(), size);
		currentSize += size;

		auto includeBufferEnd = strchr(include.data(), '\0');
		includeSize = includeBufferEnd - include.data();

		memcpy(newProgram.get() + currentSize, include.data(), includeSize);
		currentSize += includeSize;

		auto includeEnd = strchr(includeStart, '\n');

		size = (programBuffer->get() + *programSize) - includeEnd;
		memcpy(newProgram.get() + currentSize, includeEnd, size);
		currentSize += size;

		*programSize = currentSize;

		return newProgram;
	}

	bool getAndAppendInclude(std::unique_ptr<char[]>* programBuffer, u32* programSize, const std::string &includeDir, vx::sorted_vector<vx::StringID, std::string>* includeFiles)
	{
		std::unique_ptr<char[]> includeBuffer;
		u32 includeSize = 0;
		auto includeStart = strstr(programBuffer->get(), "#include");
		if (includeStart)
		{
			auto includeFile = strchr(includeStart, ' ');
			if (includeFile)
			{
				char buffer[64];
				auto strSize = sscanf(includeFile, "%s", buffer);

				auto sid = vx::make_sid(buffer + 1);

				auto it = includeFiles->find(sid);
				if (it == includeFiles->end())
				{
					std::string file = includeDir + (buffer + 1);
					file.pop_back();

					if (!loadFile(file.c_str(), includeBuffer, includeSize))
					{
						return false;
					}

					std::string includeFile;
					includeFile.assign(includeBuffer.get());

					it = includeFiles->insert(std::move(sid), std::move(includeFile));
				}

				*programBuffer = appendInclude(programBuffer, programSize, *it, includeStart);
				return true;
			}
		}

		return false;
	}

	bool loadShaderProgram(const char* programFile, const std::string &includeDir, const vx::sorted_vector<vx::StringID, vx::gl::ShaderParameter> &params, vx::gl::ShaderProgram* program, vx::sorted_vector<vx::StringID, std::string>* includeFiles)
	{
		std::unique_ptr<char[]> programBuffer;
		u32 programSize;
		if (!loadFile(programFile, programBuffer, programSize))
			return false;

		while (getAndAppendInclude(&programBuffer, &programSize, includeDir, includeFiles))
		{

		}

		while (insertParameter(&programBuffer, params))
			;

		const char* ptr = programBuffer.get();

		s32 logSize = 0;
		auto log = program->create(&ptr, logSize);
		if (log)
		{

			printf("Error '%s'\n%s\n", programFile, log.get());
			//printf("%s\n", ptr);

			return false;
		}

		return true;
	}
}

namespace vx
{
	namespace gl
	{
		struct ShaderManager::LoadUseProgramDescription
		{
			vx::gl::ProgramPipeline* pipe;
			const char* id;
			ShaderProgramType type;
			const std::string* programDir;
			const std::string* includeDir;
		};

		ShaderManager::ShaderManager()
			:m_programPipelines(),
			m_shaderPrograms(),
			m_includeFiles()
		{

		}

		ShaderManager::~ShaderManager()
		{
		}

		bool ShaderManager::loadPipelines()
		{
			auto pipelineDir = m_dataDir + "shaders/";
			auto searchMask = m_dataDir + "shaders/*.pipe";

			HANDLE fileHandle = nullptr;
			WIN32_FIND_DATAA fileData;
			if ((fileHandle = FindFirstFileA(searchMask.c_str(), &fileData)) == INVALID_HANDLE_VALUE)
			{
				//	printf("error\n");
			}

			const std::string programDir(m_dataDir + "shaders/programs/");
			const std::string includeDir = m_dataDir + "shaders/include/";

			bool result = fileHandle != INVALID_HANDLE_VALUE;
			while (result)
			{
				const bool is_directory = (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

				if (!is_directory)
				{
					auto pipelineHandle = vx::FileHandle(fileData.cFileName);
					if (!loadPipeline(pipelineHandle, pipelineDir, programDir, includeDir))
					{
						return false;
					}
				}

				result = FindNextFileA(fileHandle, &fileData);
			}

			return true;
		}

		bool ShaderManager::initialize(const std::string &dataDir, bool loadAllPipelinesFromDir)
		{
			m_dataDir = dataDir;

			if (loadAllPipelinesFromDir)
			{
				if (!loadPipelines())
				{
					printf("error loading shader pipelines\n");
					return false;
				}
			}

			return true;
		}

		void ShaderManager::clear()
		{
			m_programPipelines.clear();
			m_shaderPrograms.clear();
			m_includeFiles.clear();
		}

		bool ShaderManager::loadProgram(const FileHandle &programHandle, vx::gl::ShaderProgramType type, const std::string &programDir, const std::string &includeDir)
		{
			auto programFile = programDir + programHandle.m_string;

			auto sid = programHandle.m_sid;
			auto it = m_shaderPrograms.find(sid);
			if (it == m_shaderPrograms.end())
			{
				vx::gl::ShaderProgram program(type);
				if (!ShaderManagerCpp::loadShaderProgram(programFile.c_str(), includeDir, m_parameters, &program, &m_includeFiles))
				{
					printf("Error ShaderManager::loadProgram\n");
					return false;
				}

				m_shaderPrograms.insert(std::move(sid), std::move(program));
			}

			return true;
		}

		bool ShaderManager::useProgram(vx::gl::ProgramPipeline &pipe, const FileHandle &handle)
		{
			auto pProgram = getProgram(handle.m_sid);
			if (!pProgram)
			{
				printf("Error ShaderManager::useProgram '%s'\n", handle.m_string);
				return false;
			}

			pipe.useProgram(*pProgram);
			return true;
		}

		bool ShaderManager::loadUseProgram(const LoadUseProgramDescription &desc)
		{
			// check for not used shader stage
			if (strcmp(desc.id, "''") == 0)
			{
				return true;
			}

			auto fileHandle = FileHandle(desc.id);
			if (!loadProgram(fileHandle, desc.type, *desc.programDir, *desc.includeDir))
				return false;
			if (!useProgram(*desc.pipe, fileHandle))
				return false;

			return true;
		}

		bool ShaderManager::loadPipeline(const FileHandle &fileHandle, const std::string &pipelineDir, const std::string &programDir, const std::string &includeDir)
		{
			auto pipelineFileWithPath = pipelineDir + fileHandle.m_string;

			std::ifstream inFile(pipelineFileWithPath.c_str());
			if (!inFile.is_open())
			{
				printf("could not open pipeline file '%s'\n", pipelineFileWithPath.c_str());
				printf("'%s'\n", fileHandle.m_string);
				return false;
			}

			std::string shaders[4];
			u32 shaderIndex = 0;
			char buffer[128];
			while (!inFile.eof())
			{
				if (!inFile.getline(buffer, 128))
				{
					puts("error reading line");
					return false;
				}

				shaders[shaderIndex++] = buffer;
			}

			vx::gl::ProgramPipeline pipe;
			pipe.create();

			LoadUseProgramDescription desc;
			desc.includeDir = &includeDir;
			desc.pipe = &pipe;
			desc.programDir = &programDir;

			desc.id = shaders[0].c_str();
			desc.type = vx::gl::ShaderProgramType::VERTEX;
			if (!loadUseProgram(desc))
				return false;

			desc.id = shaders[1].c_str();
			desc.type = vx::gl::ShaderProgramType::GEOMETRY;
			if (!loadUseProgram(desc))
				return false;

			desc.id = shaders[2].c_str();
			desc.type = vx::gl::ShaderProgramType::FRAGMENT;
			if (!loadUseProgram(desc))
				return false;

			desc.id = shaders[3].c_str();
			desc.type = vx::gl::ShaderProgramType::COMPUTE;
			if (!loadUseProgram(desc))
				return false;

			auto sid = fileHandle.m_sid;

			m_programPipelines.insert(std::move(sid), std::move(pipe));

			return true;
		}

		bool ShaderManager::loadPipeline(const FileHandle &fileHandle)
		{
			auto pipelineDir = m_dataDir + "shaders/";
			const std::string programDir(m_dataDir + "shaders/programs/");
			const std::string includeDir = m_dataDir + "shaders/include/";

			return loadPipeline(fileHandle, pipelineDir, programDir, includeDir);
		}

		void ShaderManager::addParameter(const char* id, const ShaderParameter &param)
		{
			m_parameters.insert(vx::make_sid(id), param);
		}

		void ShaderManager::addParameter(const char* id, s32 value)
		{
			addParameter(id, ShaderParameter(value));
		}

		void ShaderManager::addParameter(const char* id, u32 value)
		{
			addParameter(id, ShaderParameter(value));
		}

		void ShaderManager::addParameter(const char* id, f32 value)
		{
			addParameter(id, ShaderParameter(value));
		}

		const vx::gl::ShaderProgram* ShaderManager::getProgram(const vx::StringID &sid) const
		{
			auto it = m_shaderPrograms.find(sid);
			if (it == m_shaderPrograms.end())
				return nullptr;

			return &*it;
		}

		const vx::gl::ProgramPipeline* ShaderManager::getPipeline(const char* id) const
		{
			return getPipeline(FileHandle(id));
		}

		const vx::gl::ProgramPipeline* ShaderManager::getPipeline(const FileHandle &fileHandle) const
		{
			return getPipeline(fileHandle.m_sid);
		}

		const vx::gl::ProgramPipeline* ShaderManager::getPipeline(const vx::StringID &sid) const
		{
			auto it = m_programPipelines.find(sid);
			if (it == m_programPipelines.end())
				return nullptr;

			return &*it;
		}
	}
}