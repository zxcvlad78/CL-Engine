#include "texture_rect.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Простой шейдер для текстур
namespace {
    const char* texture_vs = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;
        
        uniform mat4 projection;
        uniform mat4 model;
        
        out vec2 TexCoord;
        
        void main() {
            gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";
    
    const char* texture_fs = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        
        uniform sampler2D texture0;
        uniform vec4 modulate = vec4(1.0, 1.0, 1.0, 1.0);
        
        void main() {
            vec4 texColor = texture(texture0, TexCoord);
            FragColor = texColor * modulate;
        }
    )";
    
    // Глобальные переменные для шейдера и буферов
    GLuint texture_shader = 0;
    GLuint texture_vao = 0;
    GLuint texture_vbo = 0;
    GLuint texture_ebo = 0;
    
    bool shader_compiled = false;
    
    void compile_texture_shader() {
        if (shader_compiled) return;
        
        // Компиляция вершинного шейдера
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &texture_vs, NULL);
        glCompileShader(vertex_shader);
        
        // Проверка ошибок компиляции
        GLint success;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
        }
        
        // Компиляция фрагментного шейдера
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &texture_fs, NULL);
        glCompileShader(fragment_shader);
        
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        }
        
        // Создание шейдерной программы
        texture_shader = glCreateProgram();
        glAttachShader(texture_shader, vertex_shader);
        glAttachShader(texture_shader, fragment_shader);
        glLinkProgram(texture_shader);
        
        glGetProgramiv(texture_shader, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(texture_shader, 512, NULL, info_log);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        }
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        
        // Создание вершинного буфера
        float vertices[] = {
            // Позиции        // Текстурные координаты
            0.0f, 0.0f,       0.0f, 0.0f,  // нижний левый
            1.0f, 0.0f,       1.0f, 0.0f,  // нижний правый
            0.0f, 1.0f,       0.0f, 1.0f,  // верхний левый
            1.0f, 1.0f,       1.0f, 1.0f   // верхний правый
        };
        
        unsigned int indices[] = {
            0, 1, 2,  // первый треугольник
            1, 2, 3   // второй треугольник
        };
        
        glGenVertexArrays(1, &texture_vao);
        glGenBuffers(1, &texture_vbo);
        glGenBuffers(1, &texture_ebo);
        
        glBindVertexArray(texture_vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // Атрибут позиции
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Атрибут текстурных координат
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
        
        shader_compiled = true;
        std::cout << "Texture shader compiled successfully" << std::endl;
    }
}

TextureRect::TextureRect(const std::string& name)
    : Control(name)
{
}

void TextureRect::set_texture(const std::shared_ptr<Texture>& texture) {
    if (m_texture != texture) {
        m_texture = texture;
        queue_redraw();
    }
}

void TextureRect::set_stretch_mode(StretchMode mode) {
    if (m_stretch_mode != mode) {
        m_stretch_mode = mode;
        queue_redraw();
    }
}

void TextureRect::set_flip_h(bool flip) {
    if (m_flip_h != flip) {
        m_flip_h = flip;
        queue_redraw();
    }
}

void TextureRect::set_flip_v(bool flip) {
    if (m_flip_v != flip) {
        m_flip_v = flip;
        queue_redraw();
    }
}

void TextureRect::set_expand(bool expand) {
    if (m_expand != expand) {
        m_expand = expand;
        queue_redraw();
    }
}

void TextureRect::set_clip_contents(bool clip) {
    if (m_clip_contents != clip) {
        m_clip_contents = clip;
        queue_redraw();
    }
}

void TextureRect::_draw() {
    Control::_draw();
    
    if (!m_texture || !is_visible_in_tree()) {
        return;
    }
    
    draw_texture();
}

void TextureRect::_notification(int what) {
    switch (what) {
        case NOTIFICATION_RESIZED:
            queue_redraw();
            break;
        case NOTIFICATION_THEME_CHANGED:
            queue_redraw();
            break;
    }
}

void TextureRect::draw_texture() {
    // Компилируем шейдер если еще не скомпилирован
    compile_texture_shader();
    
    if (!texture_shader) {
        std::cerr << "Texture shader not compiled" << std::endl;
        return;
    }
    
    // Получаем позицию и размер
    glm::vec2 pos = get_position();
    glm::vec2 size = get_size();
    
    if (size.x <= 0 || size.y <= 0) {
        return;
    }
    
    // Получаем ID текстуры
    GLuint texture_id = m_texture->get_id();
    if (texture_id == 0) {
        std::cerr << "Texture ID is 0" << std::endl;
        return;
    }
    
    // Получаем исходный размер текстуры
    glm::ivec2 texture_size = m_texture->get_size();
    
    // Вычисляем область текстуры для отрисовки
    glm::vec4 tex_coords = get_draw_rect();
    
    // Настраиваем растяжение в зависимости от режима
    glm::vec2 final_size = size;
    glm::vec2 final_pos = pos;
    
    switch (m_stretch_mode) {
        case STRETCH_SCALE:
            // Просто масштабируем под размер контрола
            break;
            
        case STRETCH_KEEP:
            // Сохраняем оригинальный размер текстуры
            final_size = glm::vec2(texture_size);
            break;
            
        case STRETCH_KEEP_ASPECT: {
            // Сохраняем пропорции, вписываем в контрол
            float texture_aspect = (float)texture_size.x / texture_size.y;
            float control_aspect = size.x / size.y;
            
            if (texture_aspect > control_aspect) {
                // Ширина ограничивающая
                final_size.y = size.x / texture_aspect;
                final_pos.y = pos.y + (size.y - final_size.y) / 2.0f;
            } else {
                // Высота ограничивающая
                final_size.x = size.y * texture_aspect;
                final_pos.x = pos.x + (size.x - final_size.x) / 2.0f;
            }
            break;
        }
            
        case STRETCH_KEEP_ASPECT_CENTERED: {
            // Сохраняем пропорции, центрируем
            float texture_aspect = (float)texture_size.x / texture_size.y;
            float control_aspect = size.x / size.y;
            
            if (texture_aspect > control_aspect) {
                // Ширина ограничивающая
                final_size = glm::vec2(size.x, size.x / texture_aspect);
                final_pos = glm::vec2(pos.x, pos.y + (size.y - final_size.y) / 2.0f);
            } else {
                // Высота ограничивающая
                final_size = glm::vec2(size.y * texture_aspect, size.y);
                final_pos = glm::vec2(pos.x + (size.x - final_size.x) / 2.0f, pos.y);
            }
            break;
        }
            
        case STRETCH_KEEP_ASPECT_COVERED: {
            // Сохраняем пропорции, заполняем контрол (обрезаем)
            float texture_aspect = (float)texture_size.x / texture_size.y;
            float control_aspect = size.x / size.y;
            
            if (texture_aspect > control_aspect) {
                // Высота ограничивающая, обрезаем по ширине
                float scale = size.y / texture_size.y;
                final_size = glm::vec2(texture_size.x * scale, size.y);
                float excess_width = (final_size.x - size.x) / 2.0f;
                final_pos.x = pos.x - excess_width;
                
                // Настраиваем текстурные координаты для обрезки
                float tex_excess = excess_width / final_size.x;
                tex_coords.x += tex_excess;
                tex_coords.z -= 2.0f * tex_excess;
            } else {
                // Ширина ограничивающая, обрезаем по высоте
                float scale = size.x / texture_size.x;
                final_size = glm::vec2(size.x, texture_size.y * scale);
                float excess_height = (final_size.y - size.y) / 2.0f;
                final_pos.y = pos.y - excess_height;
                
                // Настраиваем текстурные координаты для обрезки
                float tex_excess = excess_height / final_size.y;
                tex_coords.y += tex_excess;
                tex_coords.w -= 2.0f * tex_excess;
            }
            break;
        }
            
        case STRETCH_TILE:
            // Режим плитки (упрощенная реализация)
            std::cout << "Tile stretch mode not fully implemented" << std::endl;
            break;
    }
    
    // Включаем смешивание для прозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Используем шейдер
    glUseProgram(texture_shader);
    
    // Устанавливаем матрицу проекции (орто)
    float window_width = 1280.0f;  // TODO: получить из viewport
    float window_height = 720.0f;  // TODO: получить из viewport
    glm::mat4 projection = glm::ortho(0.0f, window_width, window_height, 0.0f, -1.0f, 1.0f);
    
    GLuint proj_loc = glGetUniformLocation(texture_shader, "projection");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // Трансформация модели
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(final_pos, 0.0f));
    model = glm::scale(model, glm::vec3(final_size, 1.0f));
    
    GLuint model_loc = glGetUniformLocation(texture_shader, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Привязываем текстуру
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    
    // Устанавливаем uniform для текстуры
    GLuint texture_loc = glGetUniformLocation(texture_shader, "texture0");
    glUniform1i(texture_loc, 0);
    
    // Устанавливаем modulate цвет (по умолчанию белый)
    GLuint modulate_loc = glGetUniformLocation(texture_shader, "modulate");
    glUniform4f(modulate_loc, 1.0f, 1.0f, 1.0f, 1.0f);
    
    // Обновляем текстурные координаты в VBO в зависимости от flip режима
    float tex_vertices[] = {
        0.0f, 0.0f,   tex_coords.x, tex_coords.y,
        1.0f, 0.0f,   tex_coords.x + tex_coords.z, tex_coords.y,
        0.0f, 1.0f,   tex_coords.x, tex_coords.y + tex_coords.w,
        1.0f, 1.0f,   tex_coords.x + tex_coords.z, tex_coords.y + tex_coords.w
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_vertices), tex_vertices);
    
    // Рисуем
    glBindVertexArray(texture_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    // Отключаем смешивание
    glDisable(GL_BLEND);
    
    // Отладочный вывод
    // std::cout << "TextureRect drawn at (" << pos.x << ", " << pos.y 
    //           << ") size: " << size.x << "x" << size.y << std::endl;
}

glm::vec4 TextureRect::get_draw_rect() const {
    glm::vec4 region(0.0f, 0.0f, 1.0f, -1.0f);
    
    if (m_flip_h) {
        region.x = 1.0f;
        region.z = -1.0f;
    }
    
    if (m_flip_v) {
        region.y = 1.0f;
        region.w = 1.0f;
    }
    
    return region;
}