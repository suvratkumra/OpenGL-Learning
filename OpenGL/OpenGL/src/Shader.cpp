#include "Shader.h"
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

// we will get the file we want to parse shaders from
ShaderSourceString Shader::ParseShader(std::string path)
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


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = GLCALL(glCreateShader(type));
	const char* src = source.c_str();
	GLCALL(glShaderSource(id, 1, &src, nullptr));
	GLCALL(glCompileShader(id));

	// Error handling
	int result;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		// get the log message now.
		int length;
		GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* error_message = (char*)alloca(length * sizeof(char));
		GLCALL(glGetShaderInfoLog(id, length, &length, error_message)));
		std::cout << "Failed to compile shader: " << source.c_str() << std::endl;
		std::cout << error_message << std::endl;

		// delete the shader as it failed anyways
		GLCALL(glDeleteShader(id));

		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));

	GLCALL(glLinkProgram(program));

	GLCALL(glValidateProgram(program));


	// now the program has all the shader linked so we can dleete the shader intermediary files
	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return program;

}