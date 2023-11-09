#include "imgui.h"
#include <engine.h>

Skybox* skybox = nullptr;
CubeObject* cube = nullptr;
CubeObject* light = nullptr;
PlaneObject* plane = nullptr;

int screen_width, screen_height;
float background[] = { 0.912f, 0.912f, 0.912f, 1.0f };

// camera
Camera* camera = nullptr;
float lastX = 1280 / 2.0f;
float lastY = 720 / 2.0f;
bool firstMouse = true;
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

  // Setup imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();(void)io;

  // Imgui style
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  // Load skybox
  std::vector<std::string> skyboxFaces {
    "assets/skybox/right.jpg",
    "assets/skybox/left.jpg",
    "assets/skybox/top.jpg",
    "assets/skybox/bottom.jpg",
    "assets/skybox/front.jpg",
    "assets/skybox/back.jpg",
  };
  skybox = new Skybox(skyboxFaces);

  camera = new Camera(glm::vec3(0.0f, 1.5f, 3.0f));
  lightPos = glm::vec3(12.0f, 4.0f, 12.0f);

  // GameObjects
  cube = new CubeObject(glm::vec3(0.0f, 0.0f, 0.0f), CubeObject::TEXTURED);
  cube->setTexture(new_texture("assets/images/crate.jpg", GL_RGB), new_texture("assets/images/crate.jpg", GL_RGB));
  light = new CubeObject(lightPos, CubeObject::LIGHTING);
  plane = new PlaneObject(glm::vec3(0.0f, 0.0f, 0.0f), PlaneObject::TEXTURED);
  plane->setTexture(new_texture("assets/images/wall.jpg", GL_RGB), new_texture("assets/images/wall.jpg", GL_RGB));
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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) mouseLocked = false;
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) mouseLocked = true;

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

  ImGui::Begin("World");
  ImGui::Checkbox("Wireframe mode", &wireframeMode);
  ImGui::SliderFloat("FOV", &camera->Fov, 45.0f, 120.0f);
  ImGui::ColorEdit4("Background", background);
  ImGui::Text("Map");
  ImGui::SliderInt("CubexLen", &cubex, 1, 100);
  ImGui::SliderFloat("Objects size", &cubesize, 1.0f, 10.0f);
  ImGui::InputFloat3("Light pos", &lightPos[0]);
  ImGui::End();

  ImGui::Begin("World-GameObjects");
  ImGui::TextColored(ImVec4(1,1,0,1), "GameObjects");
  ImGui::BeginChild("GameObjects");
  int cx = cube->getPos()[0];
  int cy = cube->getPos()[1];
  int cz = cube->getPos()[2];
  ImGui::Text("Cube pos: x: %d y: %d z: %d", cx, cy, cz);
  ImGui::EndChild();
  ImGui::End();

  // Rendering functions here
  glClearColor(background[0], background[1], background[2], background[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  light->render(projection, view);
  light->setPos(lightPos);

  for (unsigned int x = 0; x < cubex; x++) {
    for (unsigned int z = 0; z < cubex; z++) {
      plane->render(projection, view);
      plane->setPos(glm::vec3(x, -1, z));
      if (x == 0 || x == cubex - 1 || z == 0 || z == cubex - 1) { cube->render(projection, view); cube->setPos(glm::vec3(x, 0, z)); }
    }
  }

  skybox->render(camera, projection, view);

  // Check all events and swap buffers
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
  glfwPollEvents();
}

int Engine::new_texture(const char *path_to_texture, GLenum format) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Load and generate the texture
  int t_width, t_height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(path_to_texture, &t_width, &t_height, &nrChannels, 0);
  if (data) {
    if (format == GL_RGBA) { glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
    else if (format == GL_RGB) { glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); }
    else { std::cout << "Invalid format" << std::endl; }
    glGenerateMipmap(GL_TEXTURE_2D);
  } else { std::cout << "Failed to load texture" << std::endl; }
  stbi_image_free(data);

  return texture;
}

void Engine::clean() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);
  cube->clean();
  skybox->clean();

  glfwTerminate();
}
