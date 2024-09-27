#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

int main(void) {
    /* Initialize the library */
    glfwInit();

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    return 0;
}