#include <engine.h>

SceneManager* sceneManager = nullptr;
TextureManager* textureManager = nullptr;
GameObject* lightcube = nullptr;
GameObject* grid = nullptr;
Skybox* skybox = nullptr;
bool showUI = false;

int screen_width, screen_height;
float background[] = { 0.912f, 0.912f, 0.912f, 1.0f };

// camera
Camera* camera = nullptr;
float lastX = 1280 / 2.0f;
float lastY = 720 / 2.0f;
bool firstMouse = !showUI;
bool mouseLocked = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, screen_width, screen_height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  if (!mouseLocked) return;
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera->ProcessMouseMovement(xoffset, yoffset);
}

Engine::Engine(int swidth, int sheight) {
  // Initialize GLFW
  glfwInit();

  screen_width = swidth;
  screen_height = sheight;

  // Configure GLFW context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 16);

  // Create new GLFW window
  window = glfwCreateWindow(swidth, sheight, "hengine", NULL, NULL);
  // Check if there are any errors while creating window
  if (window == NULL) {
    std::cout << "Failed to initialize window!" << std::endl;
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  // OPENGL Config
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  ui = new UI(window);

  sceneManager = new SceneManager();
  sceneManager->loadScene("scene", gameobjects);
  textureManager = new TextureManager();
  // Load skybox
  skybox = new Skybox(textureManager->skyboxFaces);

  camera = new Camera(glm::vec3(0.0f, 1.5f, 3.0f));
  lightPos = glm::vec3(12.0f, 4.0f, 12.0f);
  lightcube = new GameObject(lightPos, GameObject::LIGHTING);
  grid = new GameObject(glm::vec3(0.0f, -0.5f, 0.0f));
}

void Engine::update() {
  // per-frame time logic
  float currentFrame = static_cast<float>(glfwGetTime());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  
  if (mouseLocked)   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 

  // camera/view transformation
  projection = glm::perspective(glm::radians(camera->Fov), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
  view = camera->GetViewMatrix();
}

void Engine::inputs(GLFWwindow *window) {
  // Take window key inputs
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { sceneManager->saveScene("scene", gameobjects); glfwSetWindowShouldClose(window, true); }
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) { mouseLocked = true; showUI = false; }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { mouseLocked = false; showUI = true; }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera->ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera->ProcessKeyboard(DOWN, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera->Sprinting = true; else camera->Sprinting = false;
}

void Engine::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
 
  if (showUI) {
    ui->worldUI(camera, skyboxEnabled, wireframeMode, background, lightPos);
    ui->gameobjectsUI(textureManager, gameobjects, objectSelected);
    ui->inspectUI(textureManager, gameobjects, objectSelected);
    ui->toolkitUI(camera);
    ui->consoleUI();
  }

  // Rendering functions here
  glClearColor(background[0], background[1], background[2], background[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (CubeObject* obj : gameobjects) { obj->render(camera, lightPos, projection, view); }
  if (skyboxEnabled) skybox->render(camera, projection, view);

  lightcube->render(camera, lightPos, projection, view);
  lightcube->pos = lightPos;

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  grid->render(camera, lightPos, projection, view);
  grid->size = glm::vec3(12, 0, 12);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Check all events and swap buffers
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Engine::clean() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);
  for (CubeObject* obj : gameobjects) {
    obj->clean();
  }
  skybox->clean();

  glfwTerminate();
}
