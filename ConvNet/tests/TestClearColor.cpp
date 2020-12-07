#include "pch.h"
#include "TestClearColor.h"

namespace test
{
		TestClearColor::TestClearColor() 
			: m_ClearColor{ 0.8f, 0.5f, 0.8f, 1.0f }
		{}
		TestClearColor ::~TestClearColor()
		{}
		void TestClearColor::OnUpdate(float deltaTime)
		{}
		void TestClearColor::onRender()
		{
			GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3])); 
			GLCALL(glClear(GL_COLOR_BUFFER_BIT));
		}
		void TestClearColor::onImGuiRender() 
		{
			ImGui::ColorEdit4("Clear Color", m_ClearColor);
		}
	;

}