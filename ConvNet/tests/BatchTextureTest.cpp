#include "pch.h"
#include "BatchTextureTest.h"

namespace test {

    BatchTextureTest::BatchTextureTest()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)

    {
        float positions[] = {
           -50.0f, -50.0f, 0.0f, 0.0f, 0.0f,
            50.0f, -50.0f, 1.0f, 0.0f, 0.0f,
            50.0f, 50.0f, 1.0f, 1.0f, 0.0f,
            -50.0f, 50.0f, 0.0f, 1.0f, 0.0f,

           60.0f, -50.0f, 0.0f, 0.0f, 1.0f,
            110.0f, -50.0f, 1.0f, 0.0f, 1.0f,
            110.0f, 100.0f, 1.0f, 1.0f, 1.0f,
            60.0f, 100.0f, 0.0f, 1.0f, 1.0f,
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 2 * 4 * 5 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);


        m_Shader = std::make_unique<Shader>("C:\\CDEV\\OpenGLTry\\OpenGLTry\\Batch.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 0.9f);
        std::string path = "C:\\CDEV\\OpenGLTry\\OpenGLTry\\test.png";
        m_Texture = std::make_unique<Texture>(path);
        m_Shader->SetUniform1ui("u_Texture", 0);
        m_Texture->Bind();
        //    m_VAO->Unbind();
        //    m_VertexBuffer->Unbind();
        //    m_IndexBuffer->Unbind();
        //    m_Shader->Unbind();
    }
    BatchTextureTest::~BatchTextureTest()
    {}
    void BatchTextureTest::OnUpdate(float dt)
    {}
    void BatchTextureTest::onRender()
    {

        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
     /*   {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }*/
    }
    void  BatchTextureTest::onImGuiRender()
    {
        ImGui::SliderFloat("translationA x", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("translationA y", &m_TranslationA.y, 0.0f, 540.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("translationB x", &m_TranslationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("translationB y", &m_TranslationB.y, 0.0f, 540.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    }

}