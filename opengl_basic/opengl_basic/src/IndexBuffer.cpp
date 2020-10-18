#include "Render.h"
#include "Indexbuffer.h"

Indexbuffer::Indexbuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int)==sizeof(GLuint))

    GLCall(glGenBuffers(1, &mRenderer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRenderer_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(unsigned int), data, GL_STATIC_DRAW));
}

Indexbuffer::~Indexbuffer()
{
    GLCall(glDeleteBuffers(1,&mRenderer_id));
}

void Indexbuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRenderer_id));
}

void Indexbuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
