#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <intrin.h>


#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCALL(x) GLClearErrors();\
x;\
ASSERT(GLLog(#x, __FILE__, __LINE__))

static void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLog(const char* function, const char* file, int line)
{
	if (GLenum error_code = glGetError())
	{
		std::cout << std::endl << "[OpenGL error]: " << error_code << " on line: " << line << " in file: " << file;
		return false;
	}
	return true;
}


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

	ShaderSourceString shader_sources = ParseShader("res/shaders/Basic.shader");

	unsigned int shader_program = CreateShader(shader_sources.vertex_source, shader_sources.fragment_source);
	glUseProgram(shader_program);

	// Creating our 1 buffer, with "buffer" as the unique ID of this buffer, 
	// in future, we can simply tell OpenGL use this buffer and draw me whats
	// in it.
	unsigned int buffer;
	glGenBuffers(1, &buffer);

	// Selecting which buffer to use, here selecting buffer.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Tell OpenGL about the data contained in the buffer.
	// This tells opengl that the data is array buffer, with 6 floats and draw it statically
	// meaning it will contain information once but should be drawed multiple times. 
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

	// now we need to tell OpenGL one by one about what attribute is stored at what position,
	// right now we just have one position in our vertex(as it can contain other information as well)
	// we just create one attribute.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	// enable the attribute.
	glEnableVertexAttribArray(0);

	// creating element IBO
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), element_indices, GL_STATIC_DRAW);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//glBegin(GL_TRIANGLES);
		//glVertex2f(0.5, 0.5);
		//glVertex2f(0.5, -0.5);
		//glVertex2f(-0.5, 0.5);
		//glEnd();

		GLClearErrors();

		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}