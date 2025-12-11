#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <map>
#include <memory>
#include <iostream>

struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

class Label {
public:
    Label();
    Label(const std::string& text,
          std::string font_path,
          const glm::vec2& position, 
          float scale,
          const glm::vec3& color
    );
          
    ~Label();

    void set_text(const std::string& text) { m_text = text; }
    void set_position(const glm::vec2& position) { m_position = position; }
    void set_scale(float scale) { m_scale = scale; }
    void set_color(const glm::vec3& color) { m_color = color; }
    
    std::string get_text() const { return m_text; }
    glm::vec2 get_position() const { return m_position; }

    void render(GLuint shader_program);
    static bool load_font(const std::string& font_path, unsigned int font_size = 16);
    std::string get_font_path() const { return m_font_path; }
    
private:
    std::string m_text;
    std::string m_font_path;
    glm::vec2 m_position;
    float m_scale;
    glm::vec3 m_color;
    
    GLuint m_VAO;
    GLuint m_VBO;
    
    static std::map<GLchar, Character> characters;
    static bool is_initialized;
};