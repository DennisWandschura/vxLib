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
#ifndef __VX_GL_SHADERMANAGER_H
#define __VX_GL_SHADERMANAGER_H
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
			struct LoadUseProgramDescription;

			vx::sorted_vector<vx::StringID, ProgramPipeline> m_programPipelines;
			vx::sorted_vector<vx::StringID, ShaderProgram> m_shaderPrograms;
			vx::sorted_vector<vx::StringID, std::string> m_includeFiles;
			std::string m_dataDir;

			bool loadProgram(const char *file, vx::gl::ShaderProgramType type, const std::string &includeDir);
			bool useProgram(vx::gl::ProgramPipeline &pipe, const char* id);
			bool loadUseProgram(const LoadUseProgramDescription &desc);

			const vx::gl::ShaderProgram* getProgram(const char* id) const;

			bool loadPipelines();
			bool loadPipeline(const char *file, const std::string &programDir, const std::string &includeDir);

		public:
			ShaderManager();
			~ShaderManager();

			bool initialize(const std::string &dataDir, bool loadAllPipelinesFromDir);
			void clear();

			bool loadPipeline(const char *fileName);

			const vx::gl::ProgramPipeline* getPipeline(const char *id) const;
			const vx::gl::ProgramPipeline* getPipeline(const vx::StringID &sid) const;
		};
	}
}
#endif