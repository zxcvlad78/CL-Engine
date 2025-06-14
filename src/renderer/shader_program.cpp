#include "shader_program.h"
#include <iostream>

namespace Renderer
{
    ShaderProgram::ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader)
    {
        GLuint vertex_shader_ID;
        if (!create_shader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_ID)) 
        {
            std::cerr << "VERTEX SHADER Compile time error" << std::endl;
            return;
        }

        GLuint fragment_shader_ID;
        if (!create_shader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_ID))
        {
            std::cerr << "FRAGMENT SHADER Compile time error" << std::endl;
            glDeleteShader(vertex_shader_ID);
            return;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex_shader_ID);
        glAttachShader(m_ID, fragment_shader_ID);
        glLinkProgram(m_ID);

        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar info_log[1024];
            glGetShaderInfoLog(m_ID, 1024, nullptr, info_log);
            std::cerr << "ERROR::SHADER Link time error: \n" << info_log << std::endl;
        }
        else
        {
            m_is_compiled = true;
        }

        glDeleteShader(vertex_shader_ID);
        glDeleteShader(fragment_shader_ID);

    }

    bool ShaderProgram::create_shader(const std::string& source, const GLenum shader_type, GLuint& shader_ID)
    {
        shader_ID = glCreateShader(shader_type);
        const char* code = source.c_str();

        glShaderSource(shader_ID, 1, &code, nullptr);
        glCompileShader(shader_ID);

        GLint success;
        glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
        
        if (!success)
        {
            GLchar info_log[1024];
            glGetShaderInfoLog(shader_ID, 1024, nullptr, info_log);
            std::cerr << "ERROR::SHADER Compile time error: \n" << info_log << std::endl;
            return false;
        }
        
        return true;
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ID);
    }

    void ShaderProgram::use() const
    {
        glUseProgram(m_ID);
    }

    ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shader_program) noexcept
    {
        glDeleteProgram(m_ID);
        m_ID = shader_program.m_ID;
        m_is_compiled = shader_program.m_is_compiled;

        shader_program.m_ID = 0;
        shader_program.m_is_compiled = false;

        return *this;

    }
    ShaderProgram::ShaderProgram(ShaderProgram&& shader_program) noexcept
    {
        m_ID = shader_program.m_ID;
        m_is_compiled = shader_program.m_is_compiled;

        shader_program.m_ID = 0;
        shader_program.m_is_compiled = false;
    }

}