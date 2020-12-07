
#include "pch.h"
#include "renderer.h"
#include "þþIndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)  
    : m_Count(count)
{
    GLCALL(glCreateBuffers(1, &m_RendererID));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW));

}
IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
void IndexBuffer::Unbind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}
