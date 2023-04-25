#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &renderer_id_));
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &renderer_id_));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		// enable the attribute.
		GLCALL(glEnableVertexAttribArray(i));
		// now we need to tell OpenGL one by one about what attribute is stored at what position,
		// right now we just have one position in our vertex(as it can contain other information as well)
		// we just create one attribute.
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized,
			layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetTypeOfSize(element.type);
	}

}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(renderer_id_));
}
void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}