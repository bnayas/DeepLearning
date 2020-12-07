#include "pch.h"
#include "Shader.h"


Shader::Shader(const std :: string & filepath, bool computeShader)
	: m_FilePath(filepath), m_RendererID(0), IsComputeShader(computeShader)
{
    if (computeShader)
    {
        m_RendererID = CreateComputeShader(filepath);
    }
    else
    {
        ShaderComplex source = ParseShader(filepath);
        m_RendererID = CreateShader(source);
    }
}

Shader::Shader(const std::string& str, bool fromSrc, bool computeShader)
    :  m_RendererID(0), IsComputeShader(computeShader)
{
    if (fromSrc)
    {
        if (computeShader)
        {
            m_RendererID = CreateComputeShader(str, true);
        }
     }
    else {
        m_FilePath = str;
        if (computeShader)
        {
            m_RendererID = CreateComputeShader(str);
        }
        else
        {
            ShaderComplex source = ParseShader(str);
            m_RendererID = CreateShader(source);
        }
    }
}



Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_RendererID));
}


ShaderComplex Shader::ParseShader(const std::string& filepath)
{
    std::stringstream ss[2];
    //  std::cout << system("dir");
    std::ifstream stream(filepath);
    std::string line;
    int mode = -1;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                mode = 0;
            if (line.find("fragment") != std::string::npos)
                mode = 1;
        }
        else
            if (mode > -1)
                ss[mode] << line << '\n';
    }
    return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CreateShader(ShaderComplex c)
{

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, c.VertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, c.FragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::CreateComputeShader(const std::string& str, bool fromFile)
{
    std::string code;
    if (fromFile)
    {
        code = str;
    }
    else
    {
        std::stringstream ss;
        std::ifstream stream(str);
        std::string line;
        while (getline(stream, line))
            ss << line << '\n';
        code = ss.str();
    }
    unsigned int program = glCreateProgram();
    unsigned int shader = CompileShader(GL_COMPUTE_SHADER, code);
  
    glAttachShader(program, shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(shader);
 
    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        std::cout << (type == GL_VERTEX_SHADER ? "vertex" : (type == GL_FRAGMENT_SHADER ? "fragment" : "compute" )) << 
                    " shader failed to compile" << std::endl;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
    }

    return id;
}

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererID));
} 

void Shader::Call(unsigned int size_x  , 
                unsigned int size_y  , unsigned int  size_z ) const
{
    if (IsComputeShader)
    {
        for (unsigned int i = 0; i < m_ssbo.size(); i++)
        {
            GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo[i]));
 //           GLCALL(glBindBufferRange(GL_SHADER_STORAGE_BUFFER, m_ssbo_binding[i], m_ssbo[i], 0, m_buffer_size[i]));
            GLCALL(glBindBufferRange(GL_SHADER_STORAGE_BUFFER, m_ssbo_binding[i], m_ssbo[i], 0, m_buffer_size[i]));
        }
        GLCALL(glDispatchCompute(size_x, size_y, size_y));
        GLCALL(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
//        GLCALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0));
    }

}
void Shader::Unbind() const
{
    GLCALL(glUseProgram(0));
}


void Shader::SetUniform1f(const std::string& name, float v)
{
    GLCALL(glUniform1f(GetUniformLocation(name), v));

};


void Shader::SetUniform1ui(const std::string& name, unsigned int v)
{
    GLCALL(glUniform1ui(GetUniformLocation(name), v));
};
void Shader::SetUniform1i(const std::string& name, int v)
{
    GLCALL(glUniform1i(GetUniformLocation(name), v));
};

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
};

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
    GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
};

void Shader::SetUniformMat2f(const std::string& name, glm::mat2& matrix)
{
    GLCALL(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
};

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()  ));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
    m_UniformLocationCache[name] = location;
    // ASSERT(location != -1);
    return location;

}


void Shader::BindBuffer(const std::string& name)
{
    unsigned int i = m_BufferCache[name];
    unsigned ssbo = m_ssbo[i];
    GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo));
}


void Shader::ChangeBindBuffer(const std::string& name, unsigned int ssbo_binding)
{
    unsigned int i = m_BufferCache[name];
    m_ssbo_binding[i] = ssbo_binding;
}

void Shader::SetBuffer(const std::string& name, unsigned int size, unsigned int ssbo_binding)
{
    m_BufferCache[name] = (unsigned int)m_ssbo.size();
    m_ssbo.push_back((unsigned int)m_ssbo.size());
    GLCALL(glGenBuffers(1, &m_ssbo.back()));
    GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo.back()));
    GLCALL(glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_COPY));
    GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
    m_buffer_size.push_back(size);
    m_ssbo_binding.push_back(ssbo_binding);

}

void Shader::LinkBuffer(const std::string& name, const std::string& name_old, Shader* prev)
{
    unsigned int i = prev->m_BufferCache[name_old];
    m_BufferCache[name] = (unsigned int)m_ssbo.size();
    m_ssbo.push_back(prev->m_ssbo[i]);
    m_buffer_size.push_back(prev->m_buffer_size[i]);
    m_ssbo_binding.push_back(prev->m_ssbo_binding[i]);
}
