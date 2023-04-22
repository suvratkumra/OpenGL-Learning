#pragma once

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCALL(x) GLClearErrors();\
x;\
ASSERT(GLLog(#x, __FILE__, __LINE__))

void GLClearErrors();

bool GLLog(const char* function, const char* file, int line);
