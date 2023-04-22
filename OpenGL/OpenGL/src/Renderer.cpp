#include "Renderer.h"

#include "GL/glew.h"
#include <iostream>

void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLog(const char* function, const char* file, int line)
{
	if (GLenum error_code = glGetError())
	{
		std::cout << std::endl << "[OpenGL error]: " << error_code << " on line: " << line << " in file: " << file;
		return false;
	}
	return true;
}