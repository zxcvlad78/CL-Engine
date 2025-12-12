//main.cpp

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer/shader_program.h"
#include "resources/resource_manager.h"
#include "engine/engine.h"
#include "ui/label/label.h"
#include "window/window.h"

GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.3f, 0.5f, 0.0f,
    0.2f, -0.1f, 0.0f,
    -0.6f, -0.9f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};


int main(int argc, char** argv)
{
    Engine engine;
    engine.set_target_fps(144.0f);
    
    Window::Config window_config;
    window_config.title = "CL Engine";
    window_config.width = 1280;
    window_config.height = window_config.width / 16 * 9;
    window_config.vsync = false;
    
    Window window;
    
    if (!window.initialize()) {
        std::cout << "Failed to initialize window" << std::endl;
        return -1;
    }
    
    if (!window.create(window_config)) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }
    
    window.set_key_callback([&window](int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            window.set_should_close(true);
        }
    });
    
    glClearColor(0, 0, 0, 1);
    
    ResourceManager resource_manager;
    
    auto p_default_shader_program = resource_manager.load_shaders(
        "DefaultShader",
        "/res/shaders/vertex.txt",
        "/res/shaders/fragment.txt"
    );
    
    auto p_text_shader_program = resource_manager.load_shaders(
        "TextShader",
        "/res/shaders/text/vertex.txt",
        "/res/shaders/text/fragment.txt"
    );
    
    if (!p_default_shader_program || !p_text_shader_program)
    {
        std::cerr << "Can't create shader programs" << std::endl;
        return -1;
    }
    
    Label fps_label("FPS: ", "/res/fonts/default.otf", 
                    glm::vec2(8.0f, window.get_height() - 26.0f), 
                    1.0, 
                    glm::vec3(1.0f));
    
    fps_label.load_font(resource_manager.get_absolute_path("/res/fonts/default.otf"), 26.0f);
    

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window.get_width()), 
                                      0.0f, static_cast<float>(window.get_height()));
    
    GLuint text_shader_id = p_text_shader_program->get_id();
    if (text_shader_id)
    {
        p_text_shader_program->use();
        GLint projLoc = glGetUniformLocation(text_shader_id, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }
    
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    while (!window.should_close())
    {
        engine.process();
        
        fps_label.set_text("FPS: " + std::to_string(static_cast<int>(engine.get_fps())));
        
        window.clear();
        
        if (text_shader_id)
        {
            p_text_shader_program->use();
            
            GLint projLoc = glGetUniformLocation(text_shader_id, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            
            fps_label.render(text_shader_id);
        }
        
        p_default_shader_program->use();
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        window.swap_buffers();
        window.poll_events();
    }
    
    return 0;
}