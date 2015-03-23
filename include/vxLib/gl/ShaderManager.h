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
			vx::sorted_vector<vx::StringID64, ProgramPipeline> m_programPipelines;
			vx::sorted_vector<vx::StringID64, ShaderProgram> m_shaderPrograms;
			std::string m_dataDir;

			bool loadShaderInclude(const char *file);
			bool loadProgram(const char *file, vx::gl::ShaderProgramType type);
			bool useProgram(vx::gl::ProgramPipeline &pipe, const char* id);
			bool loadUseProgram(vx::gl::ProgramPipeline &pipe, const char* id, vx::gl::ShaderProgramType type, const std::string &dataDir);

			const vx::gl::ShaderProgram* getProgram(const char* id) const;

			bool loadIncludes(const std::string &dataDir);
			bool loadPipelines(const std::string &dataDir);

		public:
			ShaderManager();
			~ShaderManager();

			bool initialize(const std::string &dataDir);

			bool loadPipeline(const char *file);

			const vx::gl::ProgramPipeline* getPipeline(const char *id) const;
			const vx::gl::ProgramPipeline* getPipeline(const vx::StringID64 &sid) const;
		};
	}
}