#include "VertexArray.h"
#include "VertexBufferLayout.h" 
#include "Render.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));

}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const Vertexbuffer& vb, const VertexBufferLayout& layout)
{
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOftype(element.count);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}
