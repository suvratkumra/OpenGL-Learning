#include "Renderer.h"
#include "GL/glew.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, int size)
{
	GLCALL(glGenBuffers(1, &renderer_id_));

	// Selecting which buffer to use, here selecting buffer.
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));

	// Tell OpenGL about the data contained in the buffer.
	// This tells opengl that the data is array buffer, with 6 floats and draw it statically
	// meaning it will contain information once but should be drawed multiple times. 
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

}

VertexBuffer::~VertexBuffer()
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::Bind()
{
	// Selecting which buffer to use, here selecting buffer.
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));

}

void VertexBuffer::Unbind()
{
	// Selecting which buffer to use, here selecting buffer.
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
}
