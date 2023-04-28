#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <intrin.h>
#include <memory>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// Changing the mode of opengl to be core instead of compat
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   // using version 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);   // using version x.3 (together it makes 3.3)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewInit();

	// To get the version of opengl being used right now by the computer.
	std::cout << glGetString(GL_VERSION);

	// Position for our triangle data
	float positions[] = {
		0.5, 0.5,
		-0.5, 0.5,
		0.5, -0.5,
		-0.5, -0.5
	};

	// For glDrawElements 
	unsigned int element_indices[] = {
		0,1,2,
		1,3,2
	};


	// creating our shaders
	std::string fragment_shader;
	std::string vertex_shader;

	std::unique_ptr<Shader> shader = std::make_unique<Shader>();
	ShaderSourceString shader_sources = shader->ParseShader("res/shaders/Basic.shader");

	unsigned int shader_program = shader->CreateShader(shader_sources.vertex_source, shader_sources.fragment_source);
	GLCALL(glUseProgram(shader_program));

	std::unique_ptr<VertexArray> va = std::make_unique<VertexArray>();
	std::unique_ptr<VertexBuffer> vertex_buffer = std::make_unique<VertexBuffer>(positions, sizeof(float) * 8);
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va->AddBuffer(*vertex_buffer, layout);
	

	std::unique_ptr<IndexBuffer> index_buffer = std::make_unique<IndexBuffer>(element_indices, 6);

	// calling the shader variable and passing in a value
	GLCALL(int location_color = glGetUniformLocation(shader_program, "u_Color"));
	GLCALL(glUniform4f(location_color, 0.5, 0.5, 0.5, 1.0));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		va->Bind();
		index_buffer->Bind();
		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	index_buffer.release();
	vertex_buffer.release();

	glfwTerminate();
	return 0;
}