#include "pch.h"
#include "Window.h"


Window::Window()
{

    if (!glfwInit())
        std::cout << "Failed to init window";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    ///* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Failed to crate window";
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
        std::cout << "Error with GLEW Init" << std::endl;
    glViewport(0, 0, 800, 600);

    std::cout << glGetString(GL_VERSION) << std::endl;
}

Window::~Window()
{
    glfwTerminate();
}

void Window::EnableBlend() const
{
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

