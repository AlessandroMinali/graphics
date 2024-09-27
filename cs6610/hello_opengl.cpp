#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <stdio.h>

#define WIDTH  640
#define HEIGHT 480

double XPOS = 0;
double YPOS = 0;

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  XPOS = xpos / WIDTH;
  YPOS = ypos / HEIGHT;
}

int main(void) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);

  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetErrorCallback(error_callback);
  // glfwSwapInterval(1); // V-sync

  glfwMakeContextCurrent(window);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(XPOS,YPOS,0,1);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return 0;
}