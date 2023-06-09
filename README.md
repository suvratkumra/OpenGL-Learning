# OpenGL Learning 
 Learning OpenGL through [Learn OpenGL](https://learnopengl.com/). All the learnings from those lectures are here as push iterations
 
## Extra Header files used:
* [GLFW](https://www.glfw.org/download.html)
* [GLEW](https://glew.sourceforge.net/)

## Some Notes about OpenGL
* OpenGL is like a state machine, meaning it won't need arguments passed in it through parameters instead it already knows what it needs to do by the time you reach the draw call. 
* Vertex Buffer is just a buffer which contains the memory of where the vertices should be placed on by the vertex shader. 
* Normal draw call setup looks something like
    * Generate Buffer(with unique buffer as later if you have a lot of buffers, you just need to tell OpenGL that use this buffer with this unique ID.) 
    * Select the Buffer you want to bind to the memory.  
    * If you know what is the buffer data, then tell OpenGL about the data as well in here. But even after doing this OpenGL does not know about how to interpret the data you told it right now. 
   * To combat this issue and tell OpenGL how to read/interpret the data, we use [``glVertexAttribPointer``](https://docs.gl/gl4/glVertexAttribPointer)
   * Do not forget to enable the attribute so that it can be drawn on the screen by [``glEnableVertexAttribArray``](https://docs.gl/gl4/glEnableVertexAttribArray)
   * Now if you run the code (without the vertex and fragment shader set up, it will run fine only if the code is really simple like a triangle or something but in good practice you should write your own vertex and Fragment Shader)
   * [**Shaders**](https://learnopengl.com/Getting-started/Shaders): 
      * Vertex Shader -> Ran for every vertex in the screen / passed by CPU to GPU if clipping not enabled.
      * Fragment Shader -> Ran for every pixel between the vertex but depends on how you set your vertices up.
      * **NOTE: For this project, I made only one file for shaders (containing both fragment and vertex shaders), used stringstream and enums to easily read from the new file about frgamnet and vertex resulting in cleaner code.** 
   * Using compiler intrinsic function for error checking with OpenGL. For visual studio 2022, to set a breakpoint from within the code I have used [``__debugbreak()``](https://learn.microsoft.com/en-us/cpp/intrinsics/debugbreak?view=msvc-170), again this is compiler intrinsic and varies from compiler to compiler.
   * I have used other macros as well which are not compiler intrinsic and should work for everyone:
      * To get file path in which error occured I used [``__FILE__``](https://www.cprogramming.com/reference/preprocessor/__FILE__.html).
      * To get the line on which the error occured, I use [``__LINE__``](https://www.cprogramming.com/reference/preprocessor/__LINE__.html).
   * In this project I am using different VAO for every object instead of one global VAO for all and then binding them before rendering them in render loop. This is a personal preference and I would later change to 1 VAO for all buffers if there is performance boost in that. 
   * I have also changed my OpenGL environment to be ``CORE`` instead of ``COMPAT`` which stands for compatibility. (Core comes with no default VAO's but Compat does)
      * Using the code 
      ```
         glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   // using version 3.x
         glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);   // using version x.3 (together it makes 3.3)
         glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // setting the profile of our environment to CORE profile instead of compat
      ```
   * One thing about openGL is that when you do [``glfwTerminate()``](https://www.glfw.org/docs/3.1/group__init.html), it only cleans up opengl stuff and all the instances of any opengl object you might have, here in the code I have abstracted vertex buffer, index buffer, etc in classes and due to the main loop in ``Application.cpp`` none of them were terminated before ``glfwTerminate()``. So to fix this issue of our objects not being cleared properly we could either do:
      * Heap Allocation (making a pointer and deleting it before calling ``glfwTerminate``.
      * Wrap the entire code into a scope using curly braces ``{}``.  
