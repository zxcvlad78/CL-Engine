#include "label.h"
#include "resources/resource_manager.h"
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <sstream>

std::map<char, Label::Character> Label::characters;
std::string Label::loaded_font_path;
int Label::loaded_font_size = 0;

Label::Label(const std::string& name)
    : Control(name)
{
    setup_buffers();
}

Label::~Label() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}


void Label::set_text(const std::string& text) {
    if (m_text != text) {
        m_text = text;
        queue_redraw();
    }
}

void Label::set_font(const std::string& font_path) {
    if (m_font_path != font_path) {
        m_font_path = font_path;
        load_font();
        queue_redraw();
    }
}

void Label::set_font_size(int size) {
    if (m_font_size != size && size > 0) {
        m_font_size = size;
        load_font();
        queue_redraw();
    }
}

void Label::set_align(Align align) {
    if (m_align != align) {
        m_align = align;
        queue_redraw();
    }
}

void Label::set_valign(VAlign valign) {
    if (m_valign != valign) {
        m_valign = valign;
        queue_redraw();
    }
}

void Label::set_autowrap(bool enable) {
    if (m_autowrap != enable) {
        m_autowrap = enable;
        queue_redraw();
    }
}

void Label::set_text_color(const glm::vec4& color) {
    if (m_text_color != color) {
        m_text_color = color;
        queue_redraw();
    }
}

void Label::set_outline_size(int size) {
    if (m_outline_size != size) {
        m_outline_size = size;
        queue_redraw();
    }
}

void Label::set_outline_color(const glm::vec4& color) {
    if (m_outline_color != color) {
        m_outline_color = color;
        queue_redraw();
    }
}

void Label::_draw() {
    Control::_draw();
    
    if (m_text.empty() || !is_visible_in_tree()) {
        std::cout << "No need to render" << std::endl;
        return;
    }
    
    if (!load_font()) {
        std::cout << "Cant load font" << std::endl;
        return;
    }
    
    render_text();
}

void Label::_notification(int what) {
    switch (what) {
        case NOTIFICATION_RESIZED:
        case NOTIFICATION_THEME_CHANGED:
            queue_redraw();
            break;
    }
}

glm::vec2 Label::get_minimum_size() const {
    return calculate_text_size();
}

void Label::setup_buffers() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool Label::load_font() {
    if (m_font_path.empty() || m_font_size <= 0) {
        return false;
    }
    
    if (m_font_path == loaded_font_path && m_font_size == loaded_font_size) {
        return true;
    }
    std::cout << "Label::load_font: " << m_font_path << std::endl;
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }
    
    FT_Face face;
    if (FT_New_Face(ft, m_font_path.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " + m_font_path << std::endl;
        FT_Done_FreeType(ft);
        return false;
    }
    
    FT_Set_Pixel_Sizes(face, 0, m_font_size);
    
    // Очищаем старые символы
    for (auto& pair : characters) {
        glDeleteTextures(1, &pair.second.texture_id);
    }
    characters.clear();
    
    // Загружаем первые 128 символов ASCII
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph: " << c << std::endl;
            continue;
        }
        
        unsigned int texture;
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
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        
        characters[c] = character;
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    loaded_font_path = m_font_path;
    loaded_font_size = m_font_size;
    
    return true;
}

void Label::render_text() {
    if (characters.empty()) {
        std::cout << "'characters' is empty\n";
        return;
    }
    
    auto shader = get_shader();
    if (!shader || shader->get_id() == 0) {
        std::cerr << "ERROR: Shader not available or not initialized!" << std::endl;
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    shader->use();
    
    // Рассчитываем начальную позицию в зависимости от выравнивания
    glm::vec2 text_size = calculate_text_size();
    glm::vec2 pos = get_position();
    glm::vec2 size = get_size();
    
    float x = pos.x;
    float y = pos.y + size.y - text_size.y; // Текст рисуется снизу вверх
    
    // Горизонтальное выравнивание
    switch (m_align) {
        case ALIGN_CENTER:
        x += (size.x - text_size.x) / 2.0f;
        break;
        case ALIGN_RIGHT:
        x += size.x - text_size.x;
        break;
        case ALIGN_LEFT:
        default:
        break;
    }
    
    // Вертикальное выравнивание
    switch (m_valign) {
        case VALIGN_CENTER:
        y = pos.y + (size.y - text_size.y) / 2.0f;
        break;
        case VALIGN_TOP:
        y = pos.y;
        break;
        case VALIGN_BOTTOM:
        default:
        y = pos.y + size.y - text_size.y;
        break;
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);
    
    std::string::const_iterator c;
    for (c = m_text.begin(); c != m_text.end(); c++) {
        auto ch = characters.find(*c);
        if (ch == characters.end()) {
            continue;
        }
        
        float xpos = x + ch->second.bearing.x;
        float ypos = y - (ch->second.size.y - ch->second.bearing.y);
        
        float w = ch->second.size.x;
        float h = ch->second.size.y;
        
        // Обновляем VBO для текущего символа
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        
        glBindTexture(GL_TEXTURE_2D, ch->second.texture_id);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        x += (ch->second.advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

glm::vec2 Label::calculate_text_size() const {
    if (m_text.empty() || characters.empty()) {
        return glm::vec2(0.0f);
    }
    
    float width = 0.0f;
    float max_height = 0.0f;
    
    for (char c : m_text) {
        auto ch = characters.find(c);
        if (ch != characters.end()) {
            width += (ch->second.advance >> 6);
            max_height = std::max(max_height, static_cast<float>(ch->second.size.y));
        }
    }
    
    return glm::vec2(width, max_height);
}

std::vector<std::string> Label::wrap_text() const {
    std::vector<std::string> lines;
    if (!m_autowrap || m_text.empty()) {
        lines.push_back(m_text);
        return lines;
    }
    
    // Упрощенная реализация переноса слов
    std::istringstream iss(m_text);
    std::string word;
    std::string current_line;
    
    while (iss >> word) {
        if (current_line.length() + word.length() + 1 > 50) { // 50 - примерная ширина
            lines.push_back(current_line);
            current_line = word;
        } else {
            if (!current_line.empty()) {
                current_line += " ";
            }
            current_line += word;
        }
    }
    
    if (!current_line.empty()) {
        lines.push_back(current_line);
    }
    
    return lines;
}