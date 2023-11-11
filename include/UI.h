#ifndef UI_H
#define UI_H

#include <cubeObject.h>
#include <textureManager.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <camera.h>

class UI {
public:
  UI(GLFWwindow* window);

  void worldUI(Camera* camera, bool& skyboxEnabled, bool& wireframeMode, float background[4], glm::vec3& lightPos);
  void gameobjectsUI(TextureManager* textureManager, std::vector<CubeObject*>& gameobjects, int& objectSelected);
  void toolkitUI(Camera* camera);
  void inspectUI(TextureManager* textureManager, std::vector<CubeObject*>& gameobjects, int& objectSelected);
  void consoleUI();

private:
  unsigned int currentShader = 0;
};

#endif
