#include "pch.h"
#include "Shader.h"
#include "renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h" 
#include "þþIndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/BatchTextureTest.h"
#include "Window.h"
#include "tests/NewTest.h"

extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{
    Window win;
    win.EnableBlend();
    glfwHideWindow(win.window);

    {
        
        Renderer renderer;

        test::NewTest new_test;
        /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(win.window))
    {
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        renderer.Clear();
         
        glfwSwapBuffers(win.window);

        /* Poll for and process events */
        glfwPollEvents();
    }
     }
    
   
    return 0;
}