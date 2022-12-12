#include "VertexArray.h"
#include "glad/glad.h"
#include "../Graphics/Renderer.h"



VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind()
{
	GLCall(glBindVertexArray(m_RendererID));
}


void VertexArray::Unbind()
{
	GLCall(glBindVertexArray(0));

}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	VertexBuffer _vb = vb;
	_vb.Bind();
	const auto& _elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i< _elements.size(); ++i)
	{
		const auto& _element = _elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, _element.count, _element.type, _element.normalized, layout.GetStride(), (const void*)offset));
		offset += _element.count * VertexBufferElement::GetSizeOfType(_element.type);
	}
	
}

