#include <UI.h>
#include <vector>

UI::UI(GLFWwindow* window) {
  // Setup imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();(void)io;
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 8.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  // Imgui style
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  sceneManager = new SceneManager();
}

void UI::worldUI(Player* player, std::vector<CubeObject*>& gameobjects, bool& skyboxEnabled, bool& wireframeMode, float background[4], glm::vec3& lightPos) {
  ImGui::Begin("World");

  ImGui::Checkbox("Skybox", &skyboxEnabled);                            // Toggle skybox
  ImGui::Checkbox("Wireframe mode", &wireframeMode);                    // Toggle wireframe mode
  ImGui::SliderFloat("FOV", &player->cam->fov, 45.0f, 120.0f);          // FOV changer
  ImGui::InputFloat("Speed", &player->d_speed);                         // Camera speed changer
  if (!skyboxEnabled) ImGui::ColorEdit4("Background", background);      // Change background color if skybox is toggled off
  ImGui::InputFloat3("Light pos", &lightPos[0]);                        // Change light position

  ImGui::Spacing();
 
  ImGui::TextColored(ImVec4(0, 1, 1, 1), "Scene");
  const std::string scene_name;
  ImGui::InputText("Scene name", scene_name);                           // Choose the name of the file you want to load / save
  // Save & Load -----------------
  if (ImGui::Button("Save")) { sceneManager->saveScene(scene_name, gameobjects); }     // Save a scene
  ImGui::SameLine();
  if (ImGui::Button("Load")) { sceneManager->loadScene(scene_name, gameobjects); }     // Load a scene
  // Save & Load -----------------

  ImGui::End();
}

void UI::gameobjectsUI(TextureManager* textureManager, std::vector<CubeObject*>& gameobjects) {
  ImGui::Begin("World-GameObjects");
  ImGui::BeginChild("GameObjects");
  ImGui::TextColored(ImVec4(1,1,0,1), "GameObjects");

  ImGui::Text("Selected shader: %i", currentShader);                    // Show selected shader based on selected gameobject
  if (ImGui::Button("Textured")) currentShader = 0;                     // Select / Change current object shader
  ImGui::SameLine();
  if (ImGui::Button("Colored")) currentShader = 1;

  
  // Set object name-----------------------
  std::string name = gameobjects[objectSelected]->objectName;
  if (ImGui::InputText("Name", name)) { gameobjects[objectSelected]->objectName = name; }
  // Set object name-----------------------

  if (ImGui::Button("Add gameobject")) {
    if (currentShader == 0) {
      // Create new object with TEXTURED shader
      CubeObject* n_cb = new CubeObject(glm::vec3(0.0f, 0.0f, 0.0f), GameObject::TEXTURED);
      // Set default textures
      n_cb->setTexture(textureManager->objectTextures[1], textureManager->objectTextures[1]);
      // Add created object to the gameobjects
      gameobjects.push_back(n_cb);
    } else {
      // Create new object with COLOR shader
      CubeObject* n_cb = new CubeObject(glm::vec3(0.0f, 0.0f, 0.0f), GameObject::COLOR);
      gameobjects.push_back(n_cb);
    }
  }

  // Itterate between all gameobjects
  for (int i = 0; i < gameobjects.size(); ++i) {
    // Display / Show all gameobjects id`s
    ImGui::TextColored(ImVec4(0, 1, 1, 1), "ID: %i", i);
    ImGui::SameLine();

    // Sellect gameobject
    if (ImGui::Button("Select")) { objectSelected = i; }
  }

  ImGui::EndChild();
  ImGui::End();
}

void UI::toolkitUI(Camera* camera) {
  ImGui::Begin("Toolkit");
  // TODO: Add some random tools
  ImGui::End();
}

void UI::inspectUI(TextureManager* textureManager, std::vector<CubeObject*>& gameobjects) {
  ImGui::Begin("Inspect");
  if (objectSelected != -1) {
    ImGui::TextColored(ImVec4(0, 1, 1, 1), "Selected Object ID: %i", objectSelected);
    // Set position -----------------
    glm::vec3 pos = gameobjects[objectSelected]->pos;
    if (ImGui::InputFloat3("Position", glm::value_ptr(pos))) { gameobjects[objectSelected]->pos = pos; }
    // Set position -----------------

    // Set rotation -----------------
    glm::vec3 set_rot = gameobjects[objectSelected]->rot;
    if (ImGui::InputFloat3("Rotation", glm::value_ptr(set_rot))) { gameobjects[objectSelected]->rot = set_rot; }
    // Set rotation -----------------

    // Set size ---------------------
    glm::vec3 size = gameobjects[objectSelected]->size;
    if (ImGui::InputFloat3("Size", glm::value_ptr(size))) { gameobjects[objectSelected]->size = size; }
    // Set size ---------------------

    // Set material -----------------
    ImGui::TextColored(ImVec4(0, 1, 1, 1), "Material");
    glm::vec4 material = gameobjects[objectSelected]->material;
    if (ImGui::InputFloat4("Material", glm::value_ptr(material))) { gameobjects[objectSelected]->material = material; }
    // Set material -----------------

    // Set color --------------------
    glm::vec3 material_color = gameobjects[objectSelected]->objectColor;
    if (ImGui::ColorEdit3("Color", glm::value_ptr(material_color))) { gameobjects[objectSelected]->objectColor = material_color; }
    // Set color --------------------

    // Set shaders ------------------
    ImGui::TextColored(ImVec4(0, 1, 1, 1), "Shaders");
    ImGui::Text("Current shader: %i", gameobjects[objectSelected]->objectShader);
    ImGui::Text("Shader: ");
    ImGui::SameLine();
    if (ImGui::Button("Textured")) gameobjects[objectSelected]->objectShader = GameObject::TEXTURED;
    ImGui::SameLine();
    if (ImGui::Button("Colored")) gameobjects[objectSelected]->objectShader = GameObject::COLOR;
    // Set shaders ------------------

    // Check if selected object has TEXTURED shader
    if (gameobjects[objectSelected]->objectShader == GameObject::TEXTURED) {
      static unsigned int seltex;                                                                       // Some random value
      ImGui::TextColored(ImVec4(0, 1, 1, 1), "Current object textures");                                // Area of the text
      ImGui::Image((void*)(intptr_t)gameobjects[objectSelected]->texture1, ImVec2(60.0f, 60.0f));       // Display currently selected texture
      ImGui::SameLine();
      ImGui::Image((void*)(intptr_t)gameobjects[objectSelected]->texture2, ImVec2(60.0f, 60.0f));       // Display currently selected texture
      ImGui::Spacing();
      ImGui::TextColored(ImVec4(0, 1, 1, 1), "Textures");

      // Itterate between all textures for buttons
      for (int i = 0; i < textureManager->btnTextures.size(); ++i) {
        // Display all other available textures
        // This will create a new button for every texture in btnTextures
        if (ImGui::ImageButton((void*)(intptr_t)textureManager->btnTextures[i], ImVec2(60.0f, 60.0f))) seltex = textureManager->objectTextures[i];
        ImGui::SameLine();
      }
      ImGui::Spacing();

      if (ImGui::Button("Texture")) gameobjects[objectSelected]->texture1 = seltex;       // Overide current object texutre with the new one
      ImGui::SameLine();
      if (ImGui::Button("Texture2")) gameobjects[objectSelected]->texture2 = seltex;
    }
  }

  ImGui::End();
}

void UI::consoleUI() {
  ImGui::Begin("Console");
  ImGui::End();
}
