#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer/shader_program.h"

GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

const char* vtx_shader = 
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* frag_shader = 
"#version 460\n"
"in vec3 color;"
"out vec4 fragment_color;"
"void main() {"
"   fragment_color = vec4(color, 1.0);"
"}";

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

int main(void)
{
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

    if (!gladLoadGL()) {
        std::cout << "gladLoadGL failed" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl; 

    glClearColor(0,0,0,1);

    std::string vertex_shader(vtx_shader);
    std::string fragment_shader(frag_shader);
    Renderer::ShaderProgram shader_program(vertex_shader, fragment_shader);

    if (!shader_program.is_compiled())
    {
        std::cerr << "Cant create shader program" << std::endl;
        return -1;
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
//=====

    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        
        shader_program.use();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();

        std::cout << "GODOT 78" << std::endl;

    }

    glfwTerminate();
    return 0;
}