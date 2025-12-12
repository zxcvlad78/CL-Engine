#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
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
    
    // Uniform setters
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec2(const std::string& name, const glm::vec2& value) const;
    void set_vec3(const std::string& name, const glm::vec3& value) const;
    void set_vec4(const std::string& name, const glm::vec4& value) const;
    void set_mat2(const std::string& name, const glm::mat2& mat) const;
    void set_mat3(const std::string& name, const glm::mat3& mat) const;
    void set_mat4(const std::string& name, const glm::mat4& mat) const;
    
private:
    GLuint m_id = 0;
    mutable std::unordered_map<std::string, GLint> m_uniform_cache;
    
    GLint get_uniform_location(const std::string& name) const;
    bool check_compile_errors(GLuint shader, const std::string& type);
};

#endif // SHADER_H