#pragma once
#include "Vertexbuffer.h"
#include "VertexBufferLayout.h"
class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const Vertexbuffer& vb,const VertexBufferLayout& layout);
	void Bind() const;
	void UnBind() const;
};