#pragma once

#include <vxLib/gl/Base.h>
#include <vxLib/Container/sorted_vector.h>
#include <vxLib/StringID.h>

namespace vx
{
	namespace gl
	{
		class ProgramPipeline;
		class ShaderProgram;

		class ShaderManager
		{
			vx::sorted_vector<vx::StringID, ProgramPipeline> m_programPipelines;
			vx::sorted_vector<vx::StringID, ShaderProgram> m_shaderPrograms;
			vx::sorted_vector<vx::StringID, std::string> m_includeFiles;
			std::string m_dataDir;

			bool loadProgram(const char *file, vx::gl::ShaderProgramType type, const std::string &includeDir);
			bool useProgram(vx::gl::ProgramPipeline &pipe, const char* id);
			bool loadUseProgram(vx::gl::ProgramPipeline &pipe, const char* id, vx::gl::ShaderProgramType type, const std::string &programDir, const std::string &includeDir);

			const vx::gl::ShaderProgram* getProgram(const char* id) const;

			bool loadPipelines();
			bool loadPipeline(const char *file, const std::string &programDir, const std::string &includeDir);

		public:
			ShaderManager();
			~ShaderManager();

			bool initialize(const std::string &dataDir);

			bool loadPipeline(const char *fileName);

			const vx::gl::ProgramPipeline* getPipeline(const char *id) const;
			const vx::gl::ProgramPipeline* getPipeline(const vx::StringID &sid) const;
		};
	}
}