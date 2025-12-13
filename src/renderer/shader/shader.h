#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

class Shader {
public:
    Shader();
    Shader(const std::string& vertex_path, const std::string& fragment_path);
    ~Shader();
    
    bool load_from_files(const std::string& vertex_path, const std::string& fragment_path);
    bool load_from_strings(const std::string& vertex_code, const std::string& fragment_code);
    
    void use() const;
    GLuint get_id() const { return m_id; }
    
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec2(const std::string& name, const glm::vec2& value) const;
    void set_vec3(const std::string& name, const glm::vec3& value) const;
    void set_vec4(const std::string& name, const glm::vec4& value) const;
    void set_mat2(const std::string& name, const glm::mat2& mat) const;
    void set_mat3(const std::string& name, const glm::mat3& mat) const;
    void set_mat4(const std::string& name, const glm::mat4& mat) const;
    void set_uniform(const std::string& name, const glm::mat4& mat) {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    
    void set_uniform(const std::string& name, const glm::vec4& vec) {
        glUniform4f(glGetUniformLocation(m_id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }
    
    void set_uniform(const std::string& name, const glm::vec3& vec) {
        glUniform3f(glGetUniformLocation(m_id, name.c_str()), vec.x, vec.y, vec.z);
    }
    
    void set_uniform(const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
    }
    
    void set_uniform(const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }
private:
    GLuint m_id = 0;
    mutable std::unordered_map<std::string, GLint> m_uniform_cache;
    
    GLint get_uniform_location(const std::string& name) const;
    bool check_compile_errors(GLuint shader, const std::string& type);
};

#endif // SHADER_H