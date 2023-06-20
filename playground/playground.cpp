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
#include <playground/staticdrawable.h>
#include <glm/gtx/string_cast.hpp>

int useObjects() {
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

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark green background
    glClearColor(0.0f, 0.4f, 0.0f, 0.0f);


    // Array with the vertices for a triangle
    static const std::vector<GLfloat> g_vertex_buffer_data_triangle = {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
    };

    static const std::vector<GLfloat> g_color_buffer_data_triangle = {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f
    };

    static const std::vector<GLfloat> g_vertex_buffer_data_cube = {
        -1.0f, -1.0f, -1.0f, // triangle 1: begin
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // One color for each vertex. They were generated randomly.
    static const std::vector<GLfloat> g_color_buffer_data_cube = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    StaticDrawable cube{"shaders/defaultVertexShader.vertexshader", "shaders/defaultFragmentShader.fragmentshader"};
    cube.setVertices(g_vertex_buffer_data_cube);
    cube.setColors(g_color_buffer_data_cube);
    cube.translate(glm::mat4(1.0f));
    cube.rotate(glm::mat4(1.0f));
    cube.scale(glm::mat4(1.0f));
    cube.init();


    StaticDrawable triangle{"shaders/defaultVertexShader.vertexshader", "shaders/defaultFragmentShader.fragmentshader"};
    triangle.setVertices(g_vertex_buffer_data_triangle);
    triangle.setColors(g_color_buffer_data_triangle);
    triangle.translate(glm::mat4(1.0f));
    triangle.rotate(glm::mat4(1.0f));
    triangle.scale(glm::mat4(1.0f));
    triangle.init();


    // Projection Matrix
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);

    // for an ortho camera
//     glm::mat4 projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordiantes

    // Camera/View Matrix
    glm::mat4 viewMatrix = glm::lookAt(
                glm::vec3(4,3,-3),   // Camera is at (4,3,3) in World Space
                glm::vec3(0,0,0),   // and looks at the origin
                glm:: vec3(0,1,0)   // Head is up (set to 0, -1, 0 to look upside-down
    );

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it is colose to the camera than the former one
    glDepthFunc(GL_LESS);

    do{

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.draw(viewMatrix, projectionMatrix);
//        triangle.draw(viewMatrix, projectionMatrix);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );



    // Close OpenGL window and terminate GLFWX
    glfwTerminate();

    return 0;
}

int allInOne() {
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
    // Does any object need this?
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

    static const GLfloat g_vertex_buffer_data_cube[] = {
        -1.0f, -1.0f, -1.0f, // triangle 1: begin
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data_cube[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };


    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_cube), g_vertex_buffer_data_cube, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data_cube), g_color_buffer_data_cube, GL_STATIC_DRAW);

    ShaderLoader shaderLoader{};
    GLuint program_id_red = shaderLoader.load("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");
    GLuint program_id_blue = shaderLoader.load("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShaderBlue.fragmentshader");
    GLuint program_id_tutorial_shader = shaderLoader.load("shaders/defaultVertexShader.vertexshader", "shaders/defaultFragmentShader.fragmentshader");
    GLuint program_id = program_id_tutorial_shader;
    int counter = 0;

    // Projection Matrix
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);

    // for an ortho camera
//     glm::mat4 projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordiantes

    // Camera/View Matrix
    glm::mat4 viewMatrix = glm::lookAt(
                glm::vec3(4,3,-3),   // Camera is at (4,3,3) in World Space
                glm::vec3(0,0,0),   // and looks at the origin
                glm:: vec3(0,1,0)   // Head is up (set to 0, -1, 0 to look upside-down
    );

//    // Modle matirx: an identity matrix (model will be at the origin)
//    glm::mat4 modelMatrix = glm::mat4(1.0f);
//    // Our ModelViewProjection : multiplication of our 3 matrices
//    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    GLuint matrixID = glGetUniformLocation(program_id, "MVP");

    float z_axis_rotation = 1.0f;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it is colose to the camera than the former one
    glDepthFunc(GL_LESS);

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
        glm::vec3 rotationAxis(0.0f, 0.0f, 1.0f);
        glm::mat4 rotation = glm::rotate(z_axis_rotation, rotationAxis);
        glm::mat4 modelMatrix = translate * rotation * scale;
//         Our ModelViewProjection : multiplication of our 3 matrices
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
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                    1,          // attribute. No particular reason for 1, but must match the layout in the shader.
                    3,          // size
                    GL_FLOAT,   // type
                    GL_FALSE,   // normalized?
                    0,          // stride
                    (void*) 0   // array buffer offset
        );

        // Draw the triangle!
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12 triangles with 3 vertices each
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
        //++counter;
        z_axis_rotation += 0.025f;

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );


    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(program_id);
    glDeleteProgram(program_id_blue);
    glDeleteProgram(program_id_red);
    glDeleteProgram(program_id_tutorial_shader);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFWX
    glfwTerminate();

	return 0;
}

int main()
{
    return useObjects();
}
