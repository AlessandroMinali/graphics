# SETUP

#### Install GLFW windowing API
```bash
brew install glfw3
```

#### Usage example
```cpp
// file: main.cpp
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

int main(void) {
  glfwInit();
  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  glfwMakeContextCurrent(window);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
```

```bash
clang++ main.cpp -lglfw -framework OpenGL
```