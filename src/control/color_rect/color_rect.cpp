#include "color_rect.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace {
    // Шейдер без текстурных координат, только позиция
    const char* color_vs = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        uniform mat4 projection;
        uniform mat4 model;
        void main() {
            gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
        }
    )";
    
    const char* color_fs = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 color;
        void main() {
            FragColor = color;
        }
    )";

    GLuint color_shader = 0;
    GLuint color_vao = 0, color_vbo = 0, color_ebo = 0;
    bool color_compiled = false;

    void compile_color_shader() {
        if (color_compiled) return;

        // 1. Компиляция Vertex Shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &color_vs, NULL);
        glCompileShader(vs);

        // 2. Компиляция Fragment Shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &color_fs, NULL);
        glCompileShader(fs);

        // 3. Создание программы
        color_shader = glCreateProgram();
        glAttachShader(color_shader, vs);
        glAttachShader(color_shader, fs);
        glLinkProgram(color_shader);

        // Проверка ошибок (опционально, но рекомендуется)
        GLint success;
        glGetProgramiv(color_shader, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(color_shader, 512, NULL, infoLog);
            std::cerr << "Shader Linking Error: " << infoLog << std::endl;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        // 4. Настройка геометрии (ваш существующий код)
        float vertices[] = { 0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f,  1.0f,1.0f };
        unsigned int indices[] = { 0, 1, 2, 1, 2, 3 };

        glGenVertexArrays(1, &color_vao);
        glGenBuffers(1, &color_vbo);
        glGenBuffers(1, &color_ebo);
        glBindVertexArray(color_vao);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, color_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        color_compiled = true;
    }
}

ColorRect::ColorRect(const std::string& name) : Control(name) {}

void ColorRect::set_color(const glm::vec4& p_color) {
    m_color = p_color;
    queue_redraw();
}

void ColorRect::_draw() {
    Control::_draw();
    if (is_visible_in_tree()) {
        draw_solid_rect();
    }
}

void ColorRect::draw_solid_rect() {
    compile_color_shader();
    glUseProgram(color_shader);

    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 pos = get_position();
    glm::vec2 size = get_size();
    
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(color_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(color_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform4fv(glGetUniformLocation(color_shader, "color"), 1, glm::value_ptr(m_color));

    glBindVertexArray(color_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}