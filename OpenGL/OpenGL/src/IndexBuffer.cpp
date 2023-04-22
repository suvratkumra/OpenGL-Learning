#include "Renderer.h"
#include "GL/glew.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int *indices, int count)
{
	// Because unsigned int's can have different bytes in different operating systems.
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCALL(glGenBuffers(1, &renderer_id_));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::Bind()
{
	// Selecting which buffer to use, here selecting buffer.
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));

}

void IndexBuffer::Unbind()
{
	// Selecting which buffer to use, here selecting buffer.
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
}
