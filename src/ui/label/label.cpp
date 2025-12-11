#include "label.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <vector>
#include <map>

#include "resources/resource_manager.h"

using namespace std;

map<GLchar, Character> Label::characters;
bool Label::is_initialized = false;


Label::Label(const std::string& text = "",
    std::string font_path = "/res/fonts/Allods.ttf",
    const glm::vec2& position = glm::vec2(0.0f, 0.0f),
    float scale = 1.0f,
    const glm::vec3& color = glm::vec3(1.0f)
    )
    : m_text(text), m_position(position), m_scale(scale), m_color(color)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ResourceManager resource_manager;
    m_font_path = resource_manager.get_absolute_path(font_path);
    load_font(m_font_path);
}


Label::~Label()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}


bool Label::load_font(const std::string& font_path, unsigned int font_size)
{
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        is_initialized = false;
        return false;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, font_path.c_str(), 0, &face))
    {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_FreeType(ft);
        is_initialized = false;
        return false;
    }
    
    FT_Set_Pixel_Sizes(face, 0, font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph: " << c << std::endl;
            continue;
        }
        
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        
        characters[c] = character;


    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    is_initialized = true;
    return true;
}

void Label::render(GLuint shader_program)
{
    if (!is_initialized || m_text.empty()) {
        return;
    }
    
    GLboolean blend_enabled = glIsEnabled(GL_BLEND);
    GLboolean depth_test_enabled = glIsEnabled(GL_DEPTH_TEST);
    GLint current_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
    
    GLint blend_src, blend_dst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_src);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blend_dst);
    
    glUseProgram(shader_program);
    
    GLint textColorLoc = glGetUniformLocation(shader_program, "textColor");
    glUniform3f(textColorLoc, m_color.r, m_color.g, m_color.b);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);
    
    float x = m_position.x;
    float y = m_position.y;
    
    for (const char& c : m_text)
    {
        auto it = characters.find(c);
        if (it == characters.end()) continue;
        
        Character ch = it->second;
        
        float xpos = x + ch.bearing.x * m_scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * m_scale;
        
        float w = ch.size.x * m_scale;
        float h = ch.size.y * m_scale;
        
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        x += (ch.advance >> 6) * m_scale;
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if (!blend_enabled) glDisable(GL_BLEND);
    else glBlendFunc(blend_src, blend_dst);
    
    if (depth_test_enabled) glEnable(GL_DEPTH_TEST);
    
    glUseProgram(current_program);
}
    