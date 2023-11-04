#ifndef ENGINE_H
#define ENGINE_H 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <shader.h>
#include <stb_image.h>
#include <camera.h>

class Engine {
public:
  Engine(int swidth, int sheight); 

  void update();
  void inputs(GLFWwindow* window);
  void render();

  void framebuffer_size_callback(GLFWwindow *window, int new_width, int new_height);
  void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

  int new_texture(const char* path_to_texture, GLenum format);

  void clean();

  GLFWwindow* window;

private:
  unsigned int VBO, VAO;

  // camera
  Camera camera;
  float lastX = 1280 / 2.0f;
  float lastY = 720 / 2.0f;
  bool firstMouse = true;

  // timing
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  bool wireframeMode = false;

  unsigned int texture1, texture2;
  
  glm::vec3 cubePositions[];
};

#endif
