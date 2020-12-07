#include "pch.h"
#include "Layer.h"


Layer::Layer(unsigned int inBind , unsigned int outBind )
	: m_InputBinding(inBind), m_OutputBinding(outBind)
{
	SetOpenGLVersion(4, 3);
}
Layer::Layer()
{
	SetOpenGLVersion(4, 3);
	m_InputBinding = 0;  
	m_OutputBinding = 1;
}

Layer::~Layer()
{

}




