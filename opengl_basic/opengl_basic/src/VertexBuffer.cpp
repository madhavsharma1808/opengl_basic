#include "Render.h"
#include "Vertexbuffer.h"

Vertexbuffer::Vertexbuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1,&mRenderer_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRenderer_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER,size, data, GL_STATIC_DRAW)); //target,size of the position array,position array,usage
}

Vertexbuffer::~Vertexbuffer()
{
    GLCall(glDeleteBuffers(1,&mRenderer_id));
}

void Vertexbuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRenderer_id));
}

void Vertexbuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
