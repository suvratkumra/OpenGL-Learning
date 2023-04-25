#pragma once

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include <stdexcept>
#include <vector>
#include <GL/glew.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetTypeOfSize(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements_;
	unsigned int stride_;

public:
	VertexBufferLayout();
	

	template<typename T>
	void Push(unsigned int count)
	{
		std::runtime_error("Pushing failed");
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride_ += count * VertexBufferElement::GetTypeOfSize(GL_UNSIGNED_INT);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		elements_.push_back({ GL_FLOAT, count, GL_FALSE });
		stride_ += count * VertexBufferElement::GetTypeOfSize(GL_FLOAT);
	}

	// getters	
	inline const std::vector<VertexBufferElement> GetElements() const { return elements_; }
	inline const unsigned int GetStride() const { return stride_; }
};