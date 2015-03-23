#include <vxLib/gl/ShaderManager.h>
#include <vxLib/gl/ProgramPipeline.h>
#include <vxLib/gl/ShaderProgram.h>
#include <vxLib/StringID.h>
#include <fstream>
#include <Shlwapi.h>
#include <vxLib\gl\gl.h>

namespace
{
	std::unique_ptr<char[]> loadProgramImpl(std::ifstream &is, U32 &size)
	{
		auto start = is.tellg();
		is.seekg(0, std::ifstream::end);
		auto end = is.tellg();

		size_t sz = end - start;
		is.seekg(-static_cast<I64>(sz), std::ifstream::cur);

		auto ptr = std::make_unique<char[]>(sz + 1);

		is.read(ptr.get(), sz);
		ptr[sz] = '\0';
		size = sz;

		return ptr;
	}
}

namespace vx
{
	namespace gl
	{
		ShaderManager::ShaderManager()
			:m_programPipelines(),
			m_shaderPrograms()
		{

		}

		ShaderManager::~ShaderManager()
		{
		}

		bool ShaderManager::loadIncludes(const std::string &dataDir)
		{
			auto fullPath = dataDir + "shaders/include/";
			auto searchMask = dataDir + "shaders/include/*.glsl";

			HANDLE fileHandle = nullptr;
			WIN32_FIND_DATAA fileData;
			if ((fileHandle = FindFirstFileA(searchMask.c_str(), &fileData)) == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			bool result = fileHandle != INVALID_HANDLE_VALUE;
			while (result)
			{
				const bool is_directory = (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

				if (!is_directory)
				{
					auto file = fullPath + fileData.cFileName;
					if (!loadShaderInclude(file.c_str()))
					{
						return false;
					}
				}

				result = FindNextFileA(fileHandle, &fileData);
			}

			return true;
		}

		bool ShaderManager::loadPipelines(const std::string &dataDir)
		{
			auto fullPath = dataDir + "shaders/";
			auto searchMask = dataDir + "shaders/*.pipe";

			HANDLE fileHandle = nullptr;
			WIN32_FIND_DATAA fileData;
			if ((fileHandle = FindFirstFileA(searchMask.c_str(), &fileData)) == INVALID_HANDLE_VALUE)
			{
				//	printf("error\n");
			}

			bool result = fileHandle != INVALID_HANDLE_VALUE;
			while (result)
			{
				const bool is_directory = (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

				if (!is_directory)
				{
					auto file = fullPath + fileData.cFileName;
					if (!loadPipeline(file.c_str()))
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

			if (!loadIncludes(dataDir))
			{
				printf("error loading shader includes\n");
				return false;
			}

			if (!loadPipelines(dataDir))
			{
				printf("error loading shader pipelines\n");
				return false;
			}

			return true;
		}

		bool ShaderManager::loadShaderInclude(const char *file)
		{
			std::ifstream inFile(file);
			if (!inFile.is_open())
				return false;

			std::string id = "/";
			id += PathFindFileNameA(file);

			U32 strCount = 0;
			auto program = ::loadProgramImpl(inFile, strCount);
			if (!program)
				return false;

			glNamedStringARB(GL_SHADER_INCLUDE_ARB, id.size(), id.c_str(), strCount, program.get());

			return true;
		}

		bool ShaderManager::loadProgram(const char *file, vx::gl::ShaderProgramType type)
		{
			std::string fileName = PathFindFileNameA(file);
			auto sid = vx::make_sid(fileName.c_str());
			auto it = m_shaderPrograms.find(sid);
			if (it == m_shaderPrograms.end())
			{
				std::ifstream inFile(file);
				if (!inFile.is_open())
				{
					printf("ShaderManager::loadProgram(): Could not open file '%s'\n", file);
					return false;
				}

				U32 size = 0;
				auto ptr = ::loadProgramImpl(inFile, size);
				if (!ptr)
					return false;

				const char *p = ptr.get();

				I32 logSize = 0;
				vx::gl::ShaderProgram program(type);
				auto pLog = program.create(&p, logSize);
				if (pLog)
				{

					std::ofstream out("error_" + fileName);
					out.write(pLog.get(), logSize - 1);

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

		bool ShaderManager::loadUseProgram(vx::gl::ProgramPipeline &pipe, const char *id, vx::gl::ShaderProgramType type, const std::string &dataDir)
		{
			// check for not used shader stage
			if (strcmp(id, "''") == 0)
			{
				return true;
			}

			if (!loadProgram((dataDir + id).c_str(), type))
				return false;
			if (!useProgram(pipe, id))
				return false;

			return true;
		}

		bool ShaderManager::loadPipeline(const char *file)
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

			const std::string programDir(m_dataDir + "shaders/programs/");

			vx::gl::ProgramPipeline pipe;
			pipe.create();

			if (!loadUseProgram(pipe, shaders[0].c_str(), vx::gl::ShaderProgramType::VERTEX, programDir))
				return false;

			if (!loadUseProgram(pipe, shaders[1].c_str(), vx::gl::ShaderProgramType::GEOMETRY, programDir))
				return false;

			if (!loadUseProgram(pipe, shaders[2].c_str(), vx::gl::ShaderProgramType::FRAGMENT, programDir))
				return false;

			if (!loadUseProgram(pipe, shaders[3].c_str(), vx::gl::ShaderProgramType::COMPUTE, programDir))
				return false;

			auto sid = vx::make_sid(PathFindFileNameA(file));

			m_programPipelines.insert(sid, std::move(pipe));

			return true;
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