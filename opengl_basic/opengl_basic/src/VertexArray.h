#pragma once
#include "Vertexbuffer.h"

class VertexBufferLayout;
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