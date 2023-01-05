#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

bool process_input(GLFWwindow *window)
{
    //TODO: take input, if "ESC" / "Q" return false.
    return true;
}

int main()
{
    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 800;
    const char *TITLE = "OpenGL-test (Window only)";
    GLFWmonitor *monitor = NULL;
    GLFWwindow *share = NULL;

    bool play = true;

    glfwInit();

    // glfwCreateWindow(int width, int height, const char *title, \
    // GLFWmonitor *monitor, GLFWwindow *share);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, monitor, share);
    glfwMakeContextCurrent(window);

    while(play)
    {
        play = process_input(window);
    }
    
    glfwTerminate();

    return 0;
}
