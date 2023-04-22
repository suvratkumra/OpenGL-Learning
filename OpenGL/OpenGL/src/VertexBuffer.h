#pragma once

class VertexBuffer
{
private:
	unsigned int renderer_id_;

public:
	// Constructor 
	VertexBuffer(const void* data, int size);

	// Destructor
	~VertexBuffer();

	// To bind the vertex buffer id of the object with OpenGL 
	void Bind();

	// To Unbind the vertex buffer id of the object with OpenGl
	void Unbind();
};