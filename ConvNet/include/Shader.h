#pragma once
#include "../pch.h"
 // #include "renderer.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

struct ShaderComplex
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
private:
	bool IsComputeShader;
	unsigned int current_ssbo;
	std::vector<unsigned int> m_ssbo;
	std::vector<unsigned int> m_ssbo_binding;
	std::vector<unsigned int> m_buffer_size;
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	std::unordered_map<std::string, unsigned int> m_BufferCache;
public:
	Shader(const std::string& src, bool fromSrc, bool computeShader);
	Shader(const std::string& filepath, bool computeShader = false);
	~Shader();
	
	void Bind() const ;
	void Call(unsigned int size_x = 1,
		unsigned int size_y = 1, unsigned int  size_z = 1) const;
	void Unbind() const;

	void SetUniform4f(const std::string& name ,	float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float v);
	void SetUniform1ui(const std::string& name, unsigned int v);
	void SetUniform1i(const std::string& name, int v);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
	void SetUniformMat2f(const std::string& name, glm::mat2& matrix);
	void SetBuffer(const std::string& name, unsigned int size, unsigned int ssbo_binding);
	void LinkBuffer(const std::string& name, const std::string& name_old, Shader* prev);
	template <typename T>
	void printSSBO(std::string name, int M, int x, int y, int n_c);
	template <typename T>
	void SetBuffer(const std::string& name, T* data, unsigned int ssbo_binding);
	template <typename T>
	void WriteBuffer(const std::string& name, T& data, unsigned int offset = 0);
	template <typename T>
	void WriteBuffer(const std::string& name, T* data, unsigned int size, unsigned int offset);
	void BindBuffer(const std::string& name);
	void ChangeBindBuffer(const std::string& name, unsigned int ssbo_binding);
private:
	int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderComplex ParseShader(const std::string& filepath);
	unsigned int CreateShader(ShaderComplex c);
	unsigned int CreateComputeShader(const std::string& str, bool fromFile = false);

};


template <typename T>
void Shader::SetBuffer(const std::string& name, T* data, unsigned int ssbo_binding)
{
	m_BufferCache[name] = (unsigned int)m_ssbo.size();
	m_ssbo.push_back((unsigned int)m_ssbo.size());
	GLCALL(glGenBuffers(1, &m_ssbo.back()));
	GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo.back()));
	GLCALL(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(*data), data, GL_DYNAMIC_COPY));
	GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	m_buffer_size.push_back(sizeof(*data));
	m_ssbo_binding.push_back(ssbo_binding);
	 
}

template <typename T>
void Shader::WriteBuffer(const std::string& name, T& data, unsigned int offset)
{
	unsigned int i = m_BufferCache[name];
	GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo[i]));
	GLCALL(GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
	memcpy((char *) p + offset, &data, sizeof(data));
	GLCALL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
}

template <typename T>
void Shader::WriteBuffer(const std::string& name, T* data, unsigned int size, unsigned int offset)
{
	unsigned int i = m_BufferCache[name];
	GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo[i]));
	GLCALL(GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
	memcpy((char*)p + offset, data, size);
	GLCALL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));

	//GLCALL(T * p =(T*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
	//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//memcpy((T*)p + offset, data, size);
	//GLCALL(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
	//GLCALL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));

	//	GLCALL(T * data2 = (T*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
	//	GLCALL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));

}

template <typename T>
void Shader::printSSBO(std::string name, int M, int x, int y, int n_c)
{
	GLCALL(glMemoryBarrier(GL_ALL_BARRIER_BITS));
	GLCALL(glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT));
	auto syncObj = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); 
	glClientWaitSync(syncObj, 0, ~GLuint64(0));
	Bind();
	BindBuffer(name);
	GLCALL(T* data = (T*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));

	//    GLCALL(float *data = (float*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, M * x * y * n_c * sizeof(float), GL_MAP_READ_BIT));
	for (int m = 0; m < M; m++)
	{
		std::cout << "m: " << m << std::endl;
		for (int f = 0; f < n_c; f++)
		{
			std::cout << "filter " << f << std::endl;
			for (int i = 0; i < y; i++)
			{
				for (int j = 0; j < x; j++)
					std::cout << data[(m * n_c * x * x) + f * x * y + i * x + j] << "  ";
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}
	GLCALL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));

}