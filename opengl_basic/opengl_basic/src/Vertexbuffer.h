#pragma once

class Vertexbuffer
{
	private:
		unsigned int mRenderer_id;
public:
	Vertexbuffer(const void* data, unsigned int size);
	~Vertexbuffer();

	void Bind() const;
	void UnBind() const;

};