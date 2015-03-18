#include <vxLib\gl\ShaderProgram.h>
#include <vxLib/gl/gl.h>

namespace vx
{
	namespace gl
	{
		ShaderProgram::ShaderProgram(ShaderProgramType type)
			:Base(),
			m_type(type)
		{
		}

		ShaderProgram::ShaderProgram(ShaderProgram &&rhs) noexcept
			: Base(std::move(rhs)),
			m_type(rhs.m_type)
		{
		}

		ShaderProgram::~ShaderProgram()
		{
			destroy();
		}

		ShaderProgram& ShaderProgram::operator = (ShaderProgram &&rhs) noexcept
		{
			if (this != &rhs)
			{
				MyBase::operator=(std::move(rhs));
				std::swap(m_type, rhs.m_type);
			}

			return *this;
		}

		bool ShaderProgram::checkLinkStatus()
		{
			int bDidLink;
			glGetProgramiv(m_id, GL_LINK_STATUS, &bDidLink);

			return (bDidLink == GL_TRUE);
		}

		bool ShaderProgram::checkValidateStatuts()
		{
			glValidateProgram(m_id);
			int isValid;
			glGetProgramiv(m_id, GL_VALIDATE_STATUS, &isValid);

			return (isValid == GL_TRUE);
		}

		std::unique_ptr<char[]> ShaderProgram::getProgramInfoLog(int *pLength)
		{
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, pLength);

			std::unique_ptr<char[]> log;

			if (*pLength != 0)
			{
				log = std::make_unique<char[]>(*pLength);
				glGetProgramInfoLog(m_id, (*pLength), 0, log.get());
			}

			return log;
		}

		std::unique_ptr<char[]> ShaderProgram::create(const char **program, I32 &logLength)
		{
			std::unique_ptr<char[]> log;

			if (m_id == 0)
			{
				auto type = detail::getShaderProgramType(m_type);
				m_id = glCreateShaderProgramv(type, 1, program);
				if (m_id == 0)
					return log;

				if (!checkLinkStatus() || !checkValidateStatuts())
				{
					log = getProgramInfoLog(&logLength);
				}
			}

			return log;
		}

		void ShaderProgram::destroy()
		{
			if (m_id != 0)
			{
				glDeleteProgram(m_id);
				m_id = 0;
			}
		}
	}
}