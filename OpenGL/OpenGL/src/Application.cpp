#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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