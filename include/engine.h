#ifndef ENGINE_H
#define ENGINE_H 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <UI.h>

#include <shader.h>
#include <stb_image.h>
#include <sceneManager.h>
#include <textureManager.h>
#include <skybox.h>
#include <camera.h>
#include <cubeObject.h>
#include <planeObject.h>

class Engine {
public:
  Engine(int swidth, int sheight);

  void init_res();

  void update();
  void inputs(GLFWwindow* window);
  void render();

  void clean();

  GLFWwindow* window;

private:
  // Shader
  unsigned int VAO, VBO, colorVAO, lightVAO;
  unsigned int planeVAO, planeVBO;

  // Map
  UI* ui = nullptr;
  unsigned int skyboxTex;
  std::vector<CubeObject*> gameobjects;
  bool showObjectNameInput = false;
  int objectSelected = -1;
  bool skyboxEnabled = true;

  // Lighting
  glm::vec3 lightPos;

  // Camera
  glm::mat4 projection;
  glm::mat4 view;

  // timing
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  bool wireframeMode = false;

  unsigned int texture1, texture2;
};

#endif
