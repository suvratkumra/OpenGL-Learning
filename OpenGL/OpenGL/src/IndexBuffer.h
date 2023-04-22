#pragma once

class IndexBuffer
{
private:
	unsigned int renderer_id_;

public:
	// Constructor 
	IndexBuffer(const unsigned int* indices, int count);

	// Destructor
	~IndexBuffer();

	// To bind the vertex buffer id of the object with OpenGL 
	void Bind();

	// To Unbind the vertex buffer id of the object with OpenGl
	void Unbind();
};