#pragma once
#include "pch.h"
#include "test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Texture.h"
#include "þþIndexBuffer.h"
#include "Shader.h"
#include "../renderer.h"

struct Matrix
{
    float W[32];
};


namespace test {

    class NewTest : public test::Test
    {
    public:
        NewTest();
        ~NewTest();

        void OnUpdate(float dt) override;
        void onRender() override;
        void  onImGuiRender() override;
    private:
        glm::mat4 m_Proj, m_View;
        glm::vec3 m_TranslationA, m_TranslationB;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr <IndexBuffer> m_IndexBuffer;
        std::unique_ptr <Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        unsigned int posSSbo; 
        unsigned int velSSbo;
        unsigned int colSSbo;

    };

}