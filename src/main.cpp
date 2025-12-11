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


short window_width = 1280;
short window_height = window_width / 16 * 9;    


char NAME[] = "CL Engine";

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    window_width = width;
    window_height = height;

    glViewport(0, 0, window_width, window_height);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    
    Engine engine;
    engine.set_target_fps(144.0f); // fps ~45
    
    
    if (!glfwInit())
    {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window;
    window = glfwCreateWindow(window_width, window_height, NAME, nullptr, nullptr);
    if (!window)
    {
        std::cout << "glfwCreateWindow" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);
    
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(0);
    
    if (!gladLoadGL()) {
        std::cout << "gladLoadGL failed" << std::endl;
        return -1;
    }
    
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl; 

    glClearColor(0,0,0,1);

    {
        // SHADERS
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
            std::cerr << "Cant create default shader programs" << std::endl;
            return -1;
        }
        
        
        // Font


        Label fps_label("FPS: ", "/res/fonts/default.otf", glm::vec2(8.0f, window_height - 26.0f), 1.0, glm::vec3(1.0f) );
        fps_label.load_font(resource_manager.get_absolute_path("/res/fonts/default.otf"), 26.0f);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window_width), 
                                          0.0f, static_cast<float>(window_height));
        
        
        GLuint text_shader_id = p_text_shader_program->get_id();
        if (text_shader_id)
        {
            p_text_shader_program->use();
            GLint projLoc = glGetUniformLocation(text_shader_id, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        }

        // GL

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
        //=====

        // Game Loop
        while (!glfwWindowShouldClose(window))
        {
            engine.process();

            fps_label.set_text("FPS: " + std::to_string(static_cast<int>(engine.get_fps())));
            
            glClear(GL_COLOR_BUFFER_BIT);

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
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}