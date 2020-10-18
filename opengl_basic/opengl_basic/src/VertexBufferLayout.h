#pragma once
#include<GL/glew.h>
#include<vector>
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOftype(unsigned int type)
	{
		return sizeof(type);
	}
};

class VertexBufferLayout
{
	private:
	std::vector<VertexBufferElement> m_ELements;
	unsigned int m_Stride;
	public:
		VertexBufferLayout()
			: m_Stride(0) {}
		template<typename T>

		void Push(unsigned int count)
		{
			static_assert(false);
		}
		template<>
		void Push<float>(unsigned int count)
		{
			m_ELements.push_back({ GL_FLOAT,count,GL_FALSE });
			m_Stride +=count* sizeof(GL_FLOAT);
		}
		
		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_ELements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
			m_Stride +=count* sizeof(GLuint);
		}

		template<>
		void Push<unsigned char>( unsigned int count)
		{
			m_ELements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
			m_Stride +=count* sizeof(GLbyte);
		}

		inline const std::vector<VertexBufferElement> GetElements() const { return m_ELements; }

		inline unsigned int GetStride() const { return m_Stride; }

};