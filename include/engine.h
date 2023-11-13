#ifndef ENGINE_H
#define ENGINE_H 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <shader.h>
#include <textureManager.h>
#include <UI.h>
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

  void mouse_callback();

  void clean();

  GLFWwindow* window;

private:
  float background[4] = { 0.912f, 0.912f, 0.912f, 1.0f };

  // Instance
  TextureManager* textureManager = nullptr;
  UI* ui = nullptr;
  Camera* camera = nullptr;
  GameObject* lightcube = nullptr;
  GameObject* grid = nullptr;
  Skybox* skybox = nullptr;

  // UI
  bool showUI = false;

  // Camera
  float lastX = 1280 / 2.0f;
  float lastY = 720 / 2.0f;
  bool firstMouse = !showUI;
  bool mouseLocked = true;
  glm::mat4 projection;
  glm::mat4 view;

  // Map
  unsigned int skyboxTex;
  std::vector<CubeObject*> gameobjects;
  bool showObjectNameInput = false;
  bool skyboxEnabled = true;

  // Lighting
  glm::vec3 lightPos;

  // Timing
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  bool wireframeMode = false;
};

#endif
