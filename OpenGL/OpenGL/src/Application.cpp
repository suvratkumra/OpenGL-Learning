#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


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

static std::string CreateFragmentShaderString()
{
	std::string fragment_shader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;"
		"\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0, 0.0, 0.0, 1.0);"
		"}\n";

	return fragment_shader;
}

static std::string CreateVertexShaderString()
{
	std::string vertex_shader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;"
		"\n"
		"void main()\n"
		"{\n"
		"gl_Position = position;"
		"}\n";

	return vertex_shader;
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
	float positions[6] = {
		0.5, 0.5,
		0.5, -0.5,
		-0.5, 0.5
	};

	// creating our shaders
	std::string fragment_shader = CreateFragmentShaderString();
	std::string vertex_shader = CreateVertexShaderString();

	unsigned int shader_program = CreateShader(vertex_shader, fragment_shader);
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
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// now we need to tell OpenGL one by one about what attribute is stored at what position,
	// right now we just have one position in our vertex(as it can contain other information as well)
	// we just create one attribute.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	// enable the attribute.
	glEnableVertexAttribArray(0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);
		glVertex2f(-0.5, 0.5);
		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}