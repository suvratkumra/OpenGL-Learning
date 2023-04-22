#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <intrin.h>
#include <memory>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderSourceString
{
	std::string vertex_source;
	std::string fragment_source;
};

// we will get the file we want to parse shaders from
static ShaderSourceString ParseShader(std::string path)
{
	// read the file
	std::ifstream stream(path);
	// error checking
	if (!stream.is_open())
	{
		std::cerr << "Error opening the file " << path;
	}

	// as we have 2 shader types in the shader file, so having an array of stringstream
	std::stringstream ss[2];

	// making an enum class so that we can cast it to int later for the index of stringstream
	enum class SourceType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	// initially it is set to nun
	SourceType current_source_type = SourceType::NONE;

	// now we will start by reading from the file and first do the checking if #shader word is found or not
	std::string line;
	while (std::getline(stream, line))
	{
		// line has the line stored from the file.

		// This is just like how you would do for maps/sets, finding a word from the given string (line)
		if (line.find("#shader") != std::string::npos)
		{
			// now we want to set the stringstream accordingly for reading.
			if (line.find("vertex") != std::string::npos)
			{
				// set the source type to vertex as we want it's int value as ss's index now
				current_source_type = SourceType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// set the source type to fragment now
				current_source_type = SourceType::FRAGMENT;
			}
		}
		else
		{
			// now we have the required source stored, use that to put line in corresponding stream.
			ss[(int)current_source_type] << line << '\n';
		}
	}
	// now we have more than 1 string we want to return, so making a struct for that purpose.
	return { ss[0].str(), ss[1].str() };

}


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// get the log message now.
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* error_message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, error_message);
		std::cout << "Failed to compile shader: " << source.c_str() << std::endl;
		std::cout << error_message << std::endl;

		// delete the shader as it failed anyways
		glDeleteShader(id);

		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glValidateProgram(program);


	// now the program has all the shader linked so we can dleete the shader intermediary files
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

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

	/* Now there is no vertex buffer object as we just turned the mode to core instead of compat */
	unsigned int vao;
	GLCALL(glCreateVertexArrays(1, &vao));
	GLCALL(glBindVertexArray(vao));

	// creating our shaders
	std::string fragment_shader;
	std::string vertex_shader;

	ShaderSourceString shader_sources = ParseShader("res/shaders/Basic.shader");

	unsigned int shader_program = CreateShader(shader_sources.vertex_source, shader_sources.fragment_source);
	GLCALL(glUseProgram(shader_program));

	std::unique_ptr<VertexBuffer> vertex_buffer = std::make_unique<VertexBuffer>(positions, sizeof(float) * 8);
	
	// now we need to tell OpenGL one by one about what attribute is stored at what position,
	// right now we just have one position in our vertex(as it can contain other information as well)
	// we just create one attribute.
	GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
	// enable the attribute.
	GLCALL(glEnableVertexAttribArray(0));

	std::unique_ptr<IndexBuffer> index_buffer = std::make_unique<IndexBuffer>(element_indices, 6);

	// calling the shader variable and passing in a value
	GLCALL(int location_color = glGetUniformLocation(shader_program, "u_Color"));
	GLCALL(glUniform4f(location_color, 0.5, 0.5, 0.5, 1.0));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCALL(glBindVertexArray(vao));
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