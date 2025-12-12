#ifndef LABEL_H
#define LABEL_H

#include "control/control.h"
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <vector>

class Label : public Control {
public:
    // Структура должна быть внутри класса
    struct Character {
        unsigned int texture_id;
        glm::ivec2 size;
        glm::ivec2 bearing;
        unsigned int advance;
    };
    
    explicit Label(const std::string& name = "Label");
    ~Label() override;
    
    // Текст
    void set_text(const std::string& text);
    const std::string& get_text() const { return m_text; }
    
    // Шрифт
    void set_font(const std::string& font_path);
    const std::string& get_font() const { return m_font_path; }
    
    void set_font_size(int size);
    int get_font_size() const { return m_font_size; }
    
    // Выравнивание
    enum Align {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT,
        ALIGN_FILL
    };
    
    enum VAlign {
        VALIGN_TOP,
        VALIGN_CENTER,
        VALIGN_BOTTOM,
        VALIGN_FILL
    };
    
    void set_align(Align align);
    Align get_align() const { return m_align; }
    
    void set_valign(VAlign valign);
    VAlign get_valign() const { return m_valign; }
    
    // Перенос текста
    void set_autowrap(bool enable);
    bool has_autowrap() const { return m_autowrap; }
    
    // Цвет текста
    void set_text_color(const glm::vec4& color);
    const glm::vec4& get_text_color() const { return m_text_color; }
    
    // Контуры (outlines)
    void set_outline_size(int size);
    int get_outline_size() const { return m_outline_size; }
    
    void set_outline_color(const glm::vec4& color);
    const glm::vec4& get_outline_color() const { return m_outline_color; }
    
    // Control методы
    void _draw() override;
    void _notification(int what) override;
    
    glm::vec2 get_minimum_size() const override;
    
private:
    std::string m_text;
    std::string m_font_path;
    int m_font_size = 16;
    Align m_align = ALIGN_LEFT;
    VAlign m_valign = VALIGN_TOP;
    bool m_autowrap = false;
    glm::vec4 m_text_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    int m_outline_size = 0;
    glm::vec4 m_outline_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Кэшированные данные шрифта
    static std::map<char, Character> characters;
    static std::string loaded_font_path;
    static int loaded_font_size;
    
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    
    bool load_font();
    void setup_buffers();
    void render_text();
    glm::vec2 calculate_text_size() const;
    std::vector<std::string> wrap_text() const;
};

#endif // LABEL_H