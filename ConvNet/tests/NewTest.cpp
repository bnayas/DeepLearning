#include "pch.h"
#include "NewTest.h"
#include "ConvLayer.h"
#include "NeuralNet.h"


namespace test {

    NewTest::NewTest()

    {
        float* data;

        float A[] = { 1, 2, 3, 4 ,
                     1, 1, 1, 1, 
                     1, 0, 1, 0, 
                     0, 0, 1, 9,

                     1, 2, 53, 4 ,
                     1, 1, 1, 1,
                     1, 0, 1, 0,
                     0, 0, 1, 0 };
        float B[] = {   
                0.f, 1.f,
                1.f, 0.f,

                1.f, 1.f,
                1.f, 1.f,

        };
        float C1[] = {
                0.f, 1.f,
                1.f, 0.f
        };
        float C2[] = {
        1.f, 1.f,
        1.f, 1.f
        };
        float B2[] = {
              1.f, 0.f,
              0.f, 0.f,

              0.f, 0.f,
              0.f, 1.f,

              0.f, 0.f,
              0.f, 0.f,

              0.f, 0.f,
              1.f, 0.f
        };
        

        Matrix log = { 0, 0, 0, 0 , 0, 0, 0, 0  , 0, 0, 0, 0, 0, 0, 0, 0 };
     //   Matrix* output = new Matrix();
        float output[360] ;
        /* 12  15  18
            6  7  6
            1  6  40 */
        const std::string name = "A";

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        float new_output2[360];

            NeuralNet n;
            n.SetInitialSize(4, 4, 1);
            n.AddConvLayer(2,0,1,2);
            n.AddConvLayer(2, 0, 1, 2);
            n.WriteFilter(0, C1);
            n.WriteFilter(0, C2, 1);
            n.WriteFilters(1, B2,2);
            n.ForwardPropagation(A, 2);

            std::cout << "A:" << std::endl;
            n.layers[0]->fwd_prog->printSSBO<float>("A", 2, 3, 3, 2);
            std::cout << "A_prev:" << std::endl;
            n.layers[0]->fwd_prog->printSSBO<float>("A_prev", 2, 4, 4, 1);
            std::cout << "filters:" << std::endl;
            n.layers[0]->fwd_prog->printSSBO<float>("f", 1, 2, 2, 2);
            std::cout << std::endl << "Next Layer:" << std::endl;
            std::cout << "A:" << std::endl;
            n.layers[1]->fwd_prog->printSSBO<float>("A", 2, 2, 2, 2);
            std::cout << "A_prev:" << std::endl;
            n.layers[1]->fwd_prog->printSSBO<float>("A_prev", 2, 3, 3, 2);
            std::cout << "filters:" << std::endl;
            n.layers[1]->fwd_prog->printSSBO<float>("f", 2, 2, 2, 2);


        //float A2[] = {  5,  7,  9  ,
        //                3,  3,  3  ,
        //                1,  2,  11 ,
        //                           
        //                3,  4,  5  ,
        //                2,  1,  2  ,
        //                0,  1,  1  ,
        //                           
        //                5,  57,  59,
        //                3,  3,  3  ,
        //                1,  2,  2  ,
        //                           
        //                3,  54,  5 ,
        //                2,  1,  2  ,
        //                0,  1,  1 };
      
        GLint size;
        glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &size);
        std::cout << "GL_MAX_SHADER_STORAGE_BLOCK_SIZE is " << size / (1024*1024) << " MB." << std::endl;
        glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &size);
        std::cout << "GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS is " << size   << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &size);
        std::cout << "GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS is " << size   << std::endl;
        //unsigned int block_index = 0;
        //block_index = glGetProgramResourceIndex(compute_prog, GL_SHADER_STORAGE_BLOCK, "shader_data");
        //compute_prog.Unbind();

    }
    NewTest::~NewTest()
    {}
    void NewTest::OnUpdate(float dt)
    {}
    void NewTest::onRender()
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
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }
    
    void NewTest::onImGuiRender()
    {

    }

}

