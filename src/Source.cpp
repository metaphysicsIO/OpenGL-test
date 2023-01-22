#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#define WIDTH 1280
#define HEIGHT 720

/* COMMANDS:
 *      ESC to close window
 *
 *      RESET EVERYTHING TO DEFAULT - Press "T"
 *
 *      Wireframe - HOLD "Q", up/down to enable/disable
 *      GL_DEPTH_TEST - Hold "W", up/down to enable/disable
 * 
 * ROTATIONS:
 *      Y-Axis - HOLD "A", up/down to increase/decrease
 *      X-Axis - HOLD "S", up/down to increase/decrease
 *      Z-Axis - HOLD "D", up/down to increase/decrease
 *
 * SET:
 *      Y-Axis - Hold "C", up/down to increase/decrease
 *      X-Axis - Hold "X", up/down to increase/decrease
 *      Z-Axis - Hold "Z", up/down to increase/decrease
 *
 * CAMERA:
 *      X-rotate - HOLD "U", up/down to increase/decrease
 *      Y-rotate - HOLD "I", up/down to increase/decrease
 *      Z-rotate - HOLD "O, up/down to increase/decrease
 *
 * AUTOMATE:
 *      WEW-mode - press P, up/down to accelerate/decelerate
 *      TURN OFF WEW-mode - Press L
 */




/* TODO: This would look better in another location. */
// Vertex shader src
const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec4 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 oColor;\n"
"uniform mat4 model;\n" // model variable 
"uniform mat4 view;\n" // view variable
"uniform mat4 projection;\n" // projection var
"void main()\n"
"{\n" // P * V *M * aPos order is important.
"   gl_Position = projection * view * model * aPos;\n" //vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   oColor = aColor;\n"
"}\n\0";

// Fragment shader src
const char *fragmentShaderSource =
"#version 330 core\n"
"in vec4 oColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = oColor;\n"
"}\n\0";



void processInput(GLFWwindow *window)
{
    /*
     * Process user input.
     */

    // Close on ESC key press
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

}

void draw()
{
    /*
     * Draw primitive
     */

    // Set the mode to triangles
    GLenum mode = GL_TRIANGLES;

    // Let i be indices, put 2 * i
    GLsizei indices = 8;

    // For trinagles that DO connect
    glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);
}

static GLuint CompileShader(const std::string &source, GLuint shader_type)
{
    /*
     * Create, compiles shaders
     */

    // Shader obj
    GLuint shader_id = glCreateShader(shader_type);

    const char *src = source.c_str();

    // attach src to shader obj
    glShaderSource(shader_id, 1, &src, nullptr);

    // compile shader
    glCompileShader(shader_id);

    // return compile shader id
    return shader_id;
}

static GLuint CreateShaderProgram(const std::string &vertex_shader, const std::string &fragment_shader)
{
    /*
     * Create program object
     */

    // Compile shaders
    GLuint vertex_shader_id= CompileShader(vertex_shader, GL_VERTEX_SHADER);
    GLuint fragment_shader_id = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);

    // attach shaders to program obj
    GLuint shader_program = glCreateProgram();

    // Attach compiled shaders
    glAttachShader(shader_program, vertex_shader_id);
    glAttachShader(shader_program, fragment_shader_id);

    // Link shaders to make execuatable
    glLinkProgram(shader_program);

    // Clean house
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return shader_program;
}

int main()
{
    const char *TITLE = "OpenGL test";

    int screen_w;
    int screen_h;

    const double PI = 3.14159;
    const float toRads = PI / 180.0f;

    // Init glfw
    if(!glfwInit())
    {
        std::cout << "ERR: glfwInit()" << std::endl;
        return EXIT_FAILURE;
    }

    // Create window
    GLFWwindow *window;

    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

    // Check window was created
    if(!window)
    {
        std::cout << "ERR: window" << std::endl;
        return EXIT_FAILURE;
    }

    // Set current context to window
    glfwMakeContextCurrent(window);

    // init GLEW
    if(glewInit() != GLEW_OK)
    {
        std::cout << "ERR: GLEW_NOT_OKAY" << std::endl;
        return EXIT_FAILURE;
    }

    // First
    GLfloat v0[] = {
        // Triangle One
        -0.5, -0.5, 0.0, // indice 0
        1.0, 0.0, 0.0, // red

        -0.5, 0.5, 0.0,  // indice 1
        0.0, 1.0, 0.0, // green

        0.5, -0.5, 0.0, // indice 2
        0.0, 0.0, 1.0, // blue
        
        // Triangle Two
        0.5, 0.5, 0.0, // indice 3
        1.0, 1.0, 0.0, // yellow

        0.5, 0.5, 0, // indice 4
        1.0, 0.0, 1.0 // purple

    };
    // Element indices
    GLubyte indices[] = {
        0,1,2,
        1,2,4
    };

    // Easier to change scale.
    GLfloat SCALE = 0.2f;

    // number of components per vertex attrib
    GLint triSize = 3;

    // Create VBO, EBO, VAO
    GLuint VBO, EBO, VAO;
    // Create buffer obj
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Create, select VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind Vertex Array
    glBindVertexArray(VBO);
    glBindVertexArray(EBO);
    // Select buffer for VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Specify attributes, location, layout to GPU
    // "6 * sizeof(GLfloat)", change 6 as needed (see: v0)
    glVertexAttribPointer(0, triSize, GL_FLOAT, GL_FALSE, (triSize * 2) * sizeof(GLfloat), (GLvoid*)(0));
    glEnableVertexAttribArray(0);
    // For color
    // BUGFIXED: `3 * 4` INSTEAD OF`3 * sizeof(GLfloat)` : due to conversion issue.
    glVertexAttribPointer(1, triSize, GL_FLOAT, GL_FALSE, (triSize * 2) * sizeof(GLfloat),(GLvoid*)(3 * 4));
    glEnableVertexAttribArray(1);
    // Create and init buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(v0), v0, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // unbind VAO
    glBindVertexArray(0);

    // Create shaders
    GLuint shaderprogram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource); 

    //float ZZ = 0.0f;

    float rotation_Y = 0.0f;
    float rotation_X = 0.0f;
    float rotation_Z = 0.0f;

    float specific_Y = 0.0f;
    float specific_X = 0.0f;
    float specific_Z = 0.0f;

    float vX = 0.0f;
    float vY = 0.0f;
    float vZ = 0.0f;

    bool wewmode = false;
    float speed = 2.0f;

    while(!glfwWindowShouldClose(window))
    {
        // 2D plane positions
        glm::vec3 planePositions[] = {
            // Book (top)
            glm::vec3(-0.5f, 0.0f, 0.0f),
            glm::vec3(-0.5f, 0.0f, -0.1f),
            glm::vec3(-0.5f, 0.1f, 0.0f),
            glm::vec3(-0.5f, -0.1f, 0.0f),
            glm::vec3(-0.4f, 0.0f, 0.0f),
            glm::vec3(-0.6f, 0.0f, 0.0f),
            // Book (bottom)
            glm::vec3(-0.5f, -0.2f, 0.0f),
            glm::vec3(-0.5f, -0.2f, -0.1f),
            glm::vec3(-0.5f, -0.3f, 0.0f),
            glm::vec3(-0.5f, -0.3f, 0.0f),
            glm::vec3(-0.4f, -0.2f, 0.0f),
            glm::vec3(-0.6f, -0.2f, 0.0f),

            // The one you can move!
            glm::vec3(specific_X, specific_Y, specific_Z),
            //glm::vec3(-0.2f, 0.0f, 0.0f),

            //glm::vec3(0.0f, 0.2f, 0.0f),
            //glm::vec3(0.0f, -0.2f, 0.0f)
        };

        // Count for me
        GLuint planePosCount = sizeof(planePositions) / 12;

        // User input to help find me the right patterns.
        glm::float32 planeRoteY[] = {
            //0.0f, 180.0f, rotation_Y, 0.0f, 0.0f, 0.0f
            0.0f, 0.0f, 0.0f, 0.0f, 90.0f, 90.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 90.0f, 90.0f,
            rotation_Y
        };

        glm::float32 planeRoteX[] = {
            //0.0f, 180.0f, rotation_X, 0.0f, 0.0f, 0.0f
            0.0f, 180.0f, 90.0f, 90.0f, 0.0f, 0.0f,
            0.0f, 180.0f, 90.0f, 90.0f, 0.0f, 0.0f,
            rotation_X
        };

        glm::float32 planeRoteZ[] = {
            //0.0f, 0.0f, rotation_Z, 0.0f, 0.0f, 0.0f
            0.0f, 0.0f, 0.0f, 90.0f, 0.0f, 0.0f, 
            0.0f, 0.0f, 0.0f, 90.0f, 0.0f, 0.0f,
            rotation_Z
        };

        std::cout << "Rote(" << rotation_X << ", " << rotation_Y
                  << ", " << rotation_Z << ") -- Set(" << specific_X
                  << ", " << specific_Y << ", " << specific_Z << ")"
                  << std::endl;

        // ROTATE-Y
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                rotation_Y = rotation_Y + 1.0f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                rotation_Y = rotation_Y - 1.0f;
            }
        }

        // ROTATE-X
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                rotation_X = rotation_X + 1.0f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                rotation_X = rotation_X - 1.0f;
            }
        }

        // ROTATE-Z
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                rotation_Z = rotation_Z + 1.0f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                rotation_Z = rotation_Z - 1.0f;
            }
        }


        // SET Z
        if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                specific_Z = specific_Z + 0.01f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                specific_Z = specific_Z - 0.01f;
            }
        }

        // SET Y
        if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                specific_Y = specific_Y + 0.01f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                specific_Y = specific_Y - 0.01f;
            }
        }

        // SET X
        if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                specific_X = specific_X + 0.01f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                specific_X = specific_X - 0.01f;
            }
        }


        // Wireframe swap
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        // Set DEPTH
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                glEnable(GL_DEPTH_TEST);
                //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_TEST);
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                glDisable(GL_DEPTH_TEST);
                //glClear(GL_COLOR_BUFFER_BIT);
            }
        }

        // RESET EVERYTHING
        if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            rotation_Y = 0.0f;
            rotation_X = 0.0f;
            rotation_Z = 0.0f;
            specific_Y = 0.0f;
            specific_X = 0.0f;
            specific_Z = 0.0f;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDisable(GL_DEPTH_TEST);
            vZ = 0.0f;
            vX = 0.0f;
            vY = 0.0f;
            speed = 2.0f;
        }

        // Monitor for input
        processInput(window);

        // Turn on shader program
        glUseProgram(shaderprogram);

        // Declare identity matrix
        glm::mat4 modelMatrix(1.0f); 
        // Declare view matrix
        glm::mat4 viewMatrix(1.0f);
        // Declare projection matrix
        glm::mat4 projectionMatrix(1.0f);


        // Apply transformations
        // View space (Camera):
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f,0.0f,-1.0f));
        viewMatrix = glm::rotate(viewMatrix, vZ * toRads, glm::vec3(0.0f, 0.0f, 1.0f));
        viewMatrix = glm::rotate(viewMatrix, vX * toRads, glm::vec3(1.0f, 0.0f, 0.0f));
        viewMatrix = glm::rotate(viewMatrix, vY * toRads, glm::vec3(0.0f, 1.0f, 0.0f));

        // Camera X 
        if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                vX = vX + 2.0f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                vX = vX - 2.0f;
            }
        }

        // Camera Y
        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                vY = vY + 2.0f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                vY = vY - 2.0f;
            }
        }

        // Camera Z
        if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                vZ = vZ + 2.0f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                vZ = vZ - 2.0f;
            }
        }

        // Uncomment for rotation fun!
        //ZZ = ZZ + 2.0f;

        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            wewmode = true;
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                speed = speed + 0.1f;
            }
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                speed = speed - 0.1f;
            }
        }
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            wewmode = false;
            speed = 2.0f;
        }

        if(wewmode)
        {
            vX = vX + speed;
            vY = vY + speed;
            vZ = vZ + speed;
        }




        // Projection space ()
        // Specifying the projection used.
        // perspective(fov, width, height, near plane, far plane)  
        projectionMatrix = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

        // set uniform shader and var.
        GLuint modelLoc = glGetUniformLocation(shaderprogram, "model");
        GLuint viewLoc = glGetUniformLocation(shaderprogram, "view");
        GLuint projectionLoc = glGetUniformLocation(shaderprogram, "projection");

        // Pass transform to shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


        // Resize window + graphics
        glfwGetFramebufferSize(window, &screen_w, &screen_h);
        glViewport(0, 0, WIDTH, HEIGHT);

        // Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // BLACK
        glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_TEST);

        // Activate VAO
        glBindVertexArray(VAO);

        // Draws the triangle
        //draw(); 
        
        // 
        for(GLuint i = 0; i < planePosCount; ++i)
        {
            // declare id matrix.
            glm::mat4 modelMatrix(1.0f);

            // transformations
            modelMatrix = glm::translate(modelMatrix, planePositions[i]);
            modelMatrix = glm::rotate(modelMatrix, planeRoteY[i] * toRads, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, planeRoteX[i] * toRads, glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, planeRoteZ[i] * toRads, glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(SCALE, SCALE, SCALE));

            // Pass model transformations to shader
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

            draw();
        }


        // Disable VAO
        glBindVertexArray(0);

        // Shut off shader program
        glUseProgram(0);

        // Swap buffers(front, back)
        glfwSwapBuffers(window);

        // Poll + process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
