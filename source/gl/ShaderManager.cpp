#include <vxLib/gl/ShaderManager.h>
#include <vxLib/gl/ProgramPipeline.h>
#include <vxLib/gl/ShaderProgram.h>
#include <vxLib/StringID.h>
#include <fstream>
#include <Shlwapi.h>
#include <vxLib\gl\gl.h>

namespace
{
	bool loadFile(const char* str, std::unique_ptr<char[]> &buffer, U32 &size)
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

		buffer = std::make_unique<char[]>(size);
		inFile.read(buffer.get(), size);

		return true;
	}

	std::unique_ptr<char[]> appendInclude(const std::unique_ptr<char[]>* programBuffer, U32* programSize, const std::string &include, const char* includeStart)
	{
		auto includeSize = include.size();
		auto newProgram = std::make_unique<char[]>(*programSize + includeSize);
		U32 currentSize = 0;

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

	bool getAndAppendInclude(std::unique_ptr<char[]>* programBuffer, U32* programSize, const std::string &includeDir, vx::sorted_vector<vx::StringID64, std::string>* includeFiles)
	{
		std::unique_ptr<char[]> includeBuffer;
		U32 includeSize = 0;
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

					it = includeFiles->insert(sid, std::move(includeFile));
				}

				*programBuffer = appendInclude(programBuffer, programSize, *it, includeStart);
				return true;
			}
		}

		return false;
	}

	bool loadShaderProgram(const char* programFile, const std::string &includeDir, vx::gl::ShaderProgram* program, vx::sorted_vector<vx::StringID64, std::string>* includeFiles)
	{
		std::unique_ptr<char[]> programBuffer;
		U32 programSize;
		if (!loadFile(programFile, programBuffer, programSize))
			return false;

		while (getAndAppendInclude(&programBuffer, &programSize, includeDir, includeFiles))
		{

		}

		const char* ptr = programBuffer.get();

		I32 logSize = 0;
		auto log = program->create(&ptr, logSize);
		if (log)
		{
			printf("Error '%s'\n%s\n", programFile, log.get());
			return false;
		}

		return true;
	}
}

namespace vx
{
	namespace gl
	{
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
			auto fullPath = m_dataDir + "shaders/";
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
					auto file = fullPath + fileData.cFileName;
					if (!loadPipeline(file.c_str(), programDir, includeDir))
					{
						return false;
					}
				}

				result = FindNextFileA(fileHandle, &fileData);
			}

			return true;
		}

		bool ShaderManager::initialize(const std::string &dataDir)
		{
			m_dataDir = dataDir;

			if (!loadPipelines())
			{
				printf("error loading shader pipelines\n");
				return false;
			}

			return true;
		}

		bool ShaderManager::loadProgram(const char *file, vx::gl::ShaderProgramType type, const std::string &includeDir)
		{
			std::string fileName = PathFindFileNameA(file);

			auto sid = vx::make_sid(fileName.c_str());
			auto it = m_shaderPrograms.find(sid);
			if (it == m_shaderPrograms.end())
			{
				vx::gl::ShaderProgram program(type);
				if (!loadShaderProgram(file, includeDir, &program, &m_includeFiles))
				{
					return false;
				}

				m_shaderPrograms.insert(sid, std::move(program));
			}

			return true;
		}

		bool ShaderManager::useProgram(vx::gl::ProgramPipeline &pipe, const char *id)
		{
			auto pProgram = getProgram(id);
			if (!pProgram)
				return false;

			pipe.useProgram(*pProgram);
			return true;
		}

		bool ShaderManager::loadUseProgram(vx::gl::ProgramPipeline &pipe, const char *id, vx::gl::ShaderProgramType type, const std::string &programDir, const std::string &includeDir)
		{
			// check for not used shader stage
			if (strcmp(id, "''") == 0)
			{
				return true;
			}

			if (!loadProgram((programDir + id).c_str(), type, includeDir))
				return false;
			if (!useProgram(pipe, id))
				return false;

			return true;
		}

		bool ShaderManager::loadPipeline(const char *file, const std::string &programDir, const std::string &includeDir)
		{
			std::ifstream inFile(file);
			if (!inFile.is_open())
			{
				printf("could not open file '%s'\n", file);
				return false;
			}

			std::string shaders[4];
			U32 shaderIndex = 0;
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

			if (!loadUseProgram(pipe, shaders[0].c_str(), vx::gl::ShaderProgramType::VERTEX, programDir, includeDir))
				return false;

			if (!loadUseProgram(pipe, shaders[1].c_str(), vx::gl::ShaderProgramType::GEOMETRY, programDir, includeDir))
				return false;

			if (!loadUseProgram(pipe, shaders[2].c_str(), vx::gl::ShaderProgramType::FRAGMENT, programDir, includeDir))
				return false;

			if (!loadUseProgram(pipe, shaders[3].c_str(), vx::gl::ShaderProgramType::COMPUTE, programDir, includeDir))
				return false;

			auto sid = vx::make_sid(PathFindFileNameA(file));

			m_programPipelines.insert(sid, std::move(pipe));

			return true;
		}

		bool ShaderManager::loadPipeline(const char *fileName)
		{
			auto pipelineDir = m_dataDir + "shaders/";
			const std::string programDir(m_dataDir + "shaders/programs/");
			const std::string includeDir = m_dataDir + "shaders/include/";

			auto file = pipelineDir + fileName;

			return loadPipeline(file.c_str(), programDir, includeDir);
		}

		const vx::gl::ShaderProgram* ShaderManager::getProgram(const char *id) const
		{
			auto sid = vx::make_sid(id);
			auto it = m_shaderPrograms.find(sid);
			if (it == m_shaderPrograms.end())
				return nullptr;

			return &*it;
		}

		const vx::gl::ProgramPipeline* ShaderManager::getPipeline(const char *id) const
		{
			auto sid = vx::make_sid(id);
			return getPipeline(sid);
		}

		const vx::gl::ProgramPipeline* ShaderManager::getPipeline(const vx::StringID64 &sid) const
		{
			auto it = m_programPipelines.find(sid);
			if (it == m_programPipelines.end())
				return nullptr;

			return &*it;
		}
	}
}