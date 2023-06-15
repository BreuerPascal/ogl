#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;
int width = 1024;
int height = 768;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <playground/util/shaderloader.h>
#include <glm/gtx/string_cast.hpp>





void justMatrixComputationTests() {
    glm::mat4 matrix{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
    std::cout << glm::to_string(matrix) << std::endl;
}



int main()
{
    glewExperimental = true; // Needed in core profile
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE); // Window is not resizeable
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set OpenGL to Major Version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // set OpenGL to Minor Version 3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	// Open a window and create its OpenGL context
    window = glfwCreateWindow( width, height, "Playground", nullptr, nullptr);
    if( window == nullptr ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
    // this is the vertext array object TODO read what this thing does
    // Is is mandatory that you must do this after the context creation (I sure yes) but also before any other
    // OpenGL call? Look up tutorials
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark green background
    glClearColor(0.0f, 0.4f, 0.0f, 0.0f);


    // Array with the vertices for a triangle
    static const GLfloat g_vertex_buffer_data[] = {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
    };

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    ShaderLoader shaderLoader{};
    GLuint program_id_red = shaderLoader.load("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");
    GLuint program_id_blue = shaderLoader.load("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShaderBlue.fragmentshader");
    GLuint program_id = program_id_red;
    int counter = 0;

    // Projection Matrix
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);

    // for an ortho camera
    // glm::mat4 projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordiantes

    // Camera/View Matrix
    glm::mat4 viewMatrix = glm::lookAt(
                glm::vec3(4,3,3),   // Camera is at (4,3,3) in World Space
                glm::vec3(0,0,0),   // and looks at the origin
                glm:: vec3(0,1,0)   // Head is up (set to 0, -1, 0 to look upside-down
    );

//    // Modle matirx: an identity matrix (model will be at the origin)
//    glm::mat4 modelMatrix = glm::mat4(1.0f);
//    // Our ModelViewProjection : multiplication of our 3 matrices
//    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    GLuint matrixID = glGetUniformLocation(program_id, "MVP");

    float z_axis_rotation = 1.0f;

	do{
        if(counter == 30) {
            counter = 0;
            if(program_id == program_id_red) {
                program_id = program_id_blue;
            } else {
                program_id = program_id_red;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // use the shader
        glUseProgram(program_id);
        // Draw nothing, see you in tutorial 2 !



        // Modle matirx: an identity matrix (model will be at the origin)
        glm::mat4 translate = glm::mat4(1.0f);
        glm::mat4 scale = glm::mat4(1.0f);
        glm::vec3 rotationAxis(1.0f, 1.0f, 1.0f);
        glm::mat4 rotation = glm::rotate(z_axis_rotation, rotationAxis);
        glm::mat4 modelMatrix = translate * rotation * scale;
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;


        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
        // first attribute buffer: vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,              // attribute 0, No particualr reason for 0 bust must match the layout in the shader.
            3,              // size
            GL_FLOAT,       // type
            GL_FALSE,       // normalized?
            0,              // stride
            (void*)0        // array buffer offset
        );
        // Draw the triangle!
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
        ++counter;
        z_axis_rotation += 0.025f;

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

    justMatrixComputationTests();

	return 0;
}
