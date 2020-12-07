#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../../vendor/glm/glm/glm.hpp"
#include "../../vendor/glm/glm/gtc/matrix_transform.hpp"
#include "../../vendor/imGUI/imgui.h"
#include "../../vendor/imGUI/imgui_impl_glfw.h"
#include "../../vendor/imGUI/imgui_impl_opengl3.h"
#include "renderer.h"

class Window
{
    public:
        GLFWwindow* window;
        Window();
        ~Window();
        void EnableBlend() const;
        void Close() const;
    private:

};

