#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(void)
{
    short WIDTH = 640;
    short HEIGTH = WIDTH / 16 * 9;    

    char NAME[] = "CL Engine";


    if (!glfwInit())
    {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(WIDTH, HEIGTH, NAME, NULL, NULL);
    if (!window)
    {
        std::cout << "glfwCreateWindow" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "gladLoadGL failed" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << GLVersion.major << '.' << GLVersion.minor << std::endl;




    //std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    //std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl; 

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