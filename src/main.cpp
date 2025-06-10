#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

short window_width = 640;
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

    glClearColor(1,0,0,1);

    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}