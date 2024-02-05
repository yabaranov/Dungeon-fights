#include "VertexArray.h"

namespace RenderEngine
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept
	{
		m_id = vertexArray.m_id;
		vertexArray.m_id = 0;
		return *this;
	}

	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
	{
		m_id = vertexArray.m_id;
		vertexArray.m_id = 0;
	}

	void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
	{
		bind();
		vertexBuffer.bind();		
		const auto& layoutElements = layout.getLayoutElements();
		GLbyte* offset = nullptr;
		for (unsigned int i = 0; i < layoutElements.size(); i++)
		{
			GLuint currentAttribIndex = m_elementsCount + i;
			glEnableVertexAttribArray(currentAttribIndex);
			glVertexAttribPointer(currentAttribIndex, layoutElements[i].count, layoutElements[i].type, layoutElements[i].normalized, layout.getStride(), offset);
			offset += layoutElements[i].size;
		}

		m_elementsCount += static_cast<unsigned int>(layoutElements.size());
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}
}
