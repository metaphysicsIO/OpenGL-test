#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

int main()
{
    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 800;
    const char *TITLE = "OpenGL-test (Window only)";
    GLFWmonitor *monitor = NULL;
    GLFWwindow *share = NULL;

    glfwInit();

    // glfwCreateWindow(int width, int height, const char *title, \
    // GLFWmonitor *monitor, GLFWwindow *share);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, monitor, share);
    glfwMakeContextCurrent(window);

    while(true)
    {
        ;;
    }
    
    glfwTerminate();

    return 0;
}
