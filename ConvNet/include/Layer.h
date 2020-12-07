#pragma once
#include "pch.h"
#include "Shader.h"

enum Activation
{
	None, Relu, softmax, tanh_activation, LeakyRelu
};

class Layer
{
public:
	Layer();
	Layer(unsigned int inBind, unsigned int outBind);
	~Layer();
	virtual void CreateShaders() = 0;
	inline void SetOpenGLVersion(unsigned int major, unsigned int minor) 
	{
		OpenGL_MajorVersion = major;
		OpenGL_MinorVersion = minor;
	}
	Shader *fwd_prog,*bck_prog;

	unsigned int InBind() const{ 
		return m_InputBinding; 
	}
	unsigned int OutBind() const { 
		return m_OutputBinding; 
	}

protected:
	unsigned int OpenGL_MajorVersion;
	unsigned int OpenGL_MinorVersion;
	std::string fwd_source_code;
	std::string bck_source_code;

	unsigned int m_InputBinding;
	unsigned int m_OutputBinding;

};

