#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() {}

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path) {
    load_from_files(vertex_path, fragment_path);
}

Shader::~Shader() {
    if (m_id != 0) {
        glDeleteProgram(m_id);
    }
}

bool Shader::load_from_files(const std::string& vertex_path, const std::string& fragment_path) {
    // Чтение vertex shader
    std::string vertex_code;
    std::ifstream vertex_file(vertex_path);
    if (vertex_file.is_open()) {
        std::stringstream vertex_stream;
        vertex_stream << vertex_file.rdbuf();
        vertex_code = vertex_stream.str();
        vertex_file.close();
    } else {
        std::cerr << "ERROR::SHADER::Cannot open vertex shader file: " << vertex_path << std::endl;
        return false;
    }
    
    // Чтение fragment shader
    std::string fragment_code;
    std::ifstream fragment_file(fragment_path);
    if (fragment_file.is_open()) {
        std::stringstream fragment_stream;
        fragment_stream << fragment_file.rdbuf();
        fragment_code = fragment_stream.str();
        fragment_file.close();
    } else {
        std::cerr << "ERROR::SHADER::Cannot open fragment shader file: " << fragment_path << std::endl;
        return false;
    }
    
    return load_from_strings(vertex_code, fragment_code);
}

bool Shader::load_from_strings(const std::string& vertex_code, const std::string& fragment_code) {
    const char* vertex_shader_code = vertex_code.c_str();
    const char* fragment_shader_code = fragment_code.c_str();
    
    // Компиляция vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    if (!check_compile_errors(vertex_shader, "VERTEX")) {
        glDeleteShader(vertex_shader);
        return false;
    }
    
    // Компиляция fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    if (!check_compile_errors(fragment_shader, "FRAGMENT")) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return false;
    }
    
    // Создание shader program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex_shader);
    glAttachShader(m_id, fragment_shader);
    glLinkProgram(m_id);
    if (!check_compile_errors(m_id, "PROGRAM")) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(m_id);
        m_id = 0;
        return false;
    }
    
    // Удаляем shaders после линковки
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return true;
}

void Shader::use() const {
    glUseProgram(m_id);
}

GLint Shader::get_uniform_location(const std::string& name) const {
    if (m_uniform_cache.find(name) != m_uniform_cache.end()) {
        return m_uniform_cache[name];
    }
    
    GLint location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1) {
        std::cerr << "WARNING::SHADER::Uniform " << name << " not found!" << std::endl;
    }
    
    m_uniform_cache[name] = location;
    return location;
}

void Shader::set_bool(const std::string& name, bool value) const {
    glUniform1i(get_uniform_location(name), (int)value);
}

void Shader::set_int(const std::string& name, int value) const {
    glUniform1i(get_uniform_location(name), value);
}

void Shader::set_float(const std::string& name, float value) const {
    glUniform1f(get_uniform_location(name), value);
}

void Shader::set_vec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(get_uniform_location(name), 1, glm::value_ptr(value));
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(get_uniform_location(name), 1, glm::value_ptr(value));
}

void Shader::set_vec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(get_uniform_location(name), 1, glm::value_ptr(value));
}

void Shader::set_mat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::set_mat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::set_mat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
}

bool Shader::check_compile_errors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar info_log[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                      << info_log << "\n" << std::endl;
            return false;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                      << info_log << "\n" << std::endl;
            return false;
        }
    }
    return true;
}