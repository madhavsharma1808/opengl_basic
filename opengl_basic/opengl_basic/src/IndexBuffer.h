#pragma once

class Indexbuffer
{
	private:
		unsigned int mRenderer_id;
		unsigned int m_Count;
public:
	Indexbuffer(const unsigned int* data, unsigned int size);
	~Indexbuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; }

};