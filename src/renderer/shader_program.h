#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer
{
    class ShaderProgram
    {
    public:
        ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader);
        ~ShaderProgram();

        bool is_compiled() const { return m_is_compiled; }
        void use() const;

        ShaderProgram() = delete;
        ShaderProgram(ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&& shader_program) noexcept;
        ShaderProgram(ShaderProgram&& shader_program) noexcept;

    private:
        bool create_shader(const std::string& source, const GLenum shader_type, GLuint& shader_ID);

        bool m_is_compiled = false;
        GLuint m_ID = 0;
    };

}