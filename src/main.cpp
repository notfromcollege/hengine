#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <shader.h>
#include <stb_image.h>

using namespace std;

unsigned int SWIDTH = 1280;
unsigned int SHEIGHT = 720;

bool wireframeMode = false;

void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height) {
  SWIDTH = new_width;
  SHEIGHT = new_height;
  glViewport(0, 0, SWIDTH, SHEIGHT);
}

void processInput(GLFWwindow *window) {
  // Take window key inputs
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int new_texture(const char* path_to_texture, GLenum format) {
  unsigned int texture; 
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // Load and generate the texture
  int t_width, t_height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path_to_texture, &t_width, &t_height, &nrChannels, 0);
  if (data) {
    if (format == GL_RGBA) { glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
    else if (format == GL_RGB) { glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); }
    else { cout << "Invalid format" << endl; }
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "Failed to load texture" << endl;
  }
  stbi_image_free(data);

  return texture;
}

int main (int argc, char *argv[]) {
  // Initialize GLFW
  glfwInit();

  // Configure GLFW context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create new GLFW window
  GLFWwindow* window = glfwCreateWindow(SWIDTH, SHEIGHT, "hengine", NULL, NULL);
  // Check if there are any errors while creating window
  if (window == NULL) {
    cout << "Failed to initialize window!" << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }
  // Set rendering view dimensions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Setup imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  // Imgui style
  ImGui::StyleColorsDark();
  
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
  
  // Create shader program
  Shader m_shader("shaders/shader.vs", "shaders/shader.fs");

  float vertices[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
  };
  
  unsigned int indices[] = {  
    0, 1, 3, // first triangle
    1, 2, 3,  // second triangle
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // Texture attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int texture1, texture2;

  texture1 = new_texture("assets/images/crate.jpg", GL_RGB);
  texture2 = new_texture("assets/images/awesomeface.png", GL_RGBA);

  m_shader.use(); 
  m_shader.setInt("texture1", 0);
  m_shader.setInt("texture2", 1);

  float color[4] = { 2.0f, 6.0f, 8.0f, 1.0f };

  // Main engine loop
  while (!glfwWindowShouldClose(window)) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Cube");
      ImGui::ColorEdit4("Color", color);
      ImGui::Checkbox("Wireframe mode", &wireframeMode);
    ImGui::End();
    // glUseProgram(shaderProgram);
    // glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

    // Rendering functions here

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Convert to normalized coordinates
    float normalizedX = (float)xpos / (float)SWIDTH * 2.0f - 1.0f;
    float normalizedY = 1.0f - (float)ypos / (float)SHEIGHT * 2.0f;
    // create transformations
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    transform = glm::translate(transform, glm::vec3(normalizedX, normalizedY, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime()/6.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Draw triangle
    m_shader.use();
    unsigned int transformLoc = glGetUniformLocation(m_shader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Check all events and swap buffers
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}
