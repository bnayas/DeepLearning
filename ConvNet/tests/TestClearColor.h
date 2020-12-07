#pragma once
#include "test.h"
#include "../include/renderer.h"
namespace test
{
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		float m_ClearColor[4];
	};

}	