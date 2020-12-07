#pragma once
#include "../pch.h"
#include "renderer.h"
class Texture
{
public:
	Texture(std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind();

	inline int GetWidth() const { return m_Width;  };
	inline int GetHeight() const { return m_Height;  };
private:
	unsigned m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Height, m_Width, m_BPP;
};
