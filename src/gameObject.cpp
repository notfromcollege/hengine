#include <gameObject.h>

// Constructor for textured object
GameObject::GameObject(ObjectType objectType, Shader* shader, unsigned int texture1, unsigned int texture2)
    : type(objectType), shader(shader), texture1(texture1), texture2(texture2) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  if (objectType == CUBE) {
    verticesSize = sizeof(cubeVertices);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, cubeVertices, GL_STATIC_DRAW);
  }
  else if (objectType == PLANE) {
    verticesSize = sizeof(planeVertices);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, planeVertices, GL_STATIC_DRAW);
  }

  shader->use();
  shader->setInt("texture1", 0);
  shader->setInt("texture2", 1);

  objectSetup();
}

// Constructor for color object
GameObject::GameObject(ObjectType objectType, Shader* shader) : type(objectType), shader(shader) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
 
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  if (objectType == CUBE) {
    verticesSize = sizeof(cubeVertices);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, cubeVertices, GL_STATIC_DRAW);
  }
  else if (objectType == PLANE) {
    verticesSize = sizeof(planeVertices);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, planeVertices, GL_STATIC_DRAW);
  }

  texture1 = 0;
  texture2 = 0;

  objectSetup();
}

GameObject::~GameObject() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void GameObject::objectSetup() {
  if (type == CUBE) {
    // Set up position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }
  else if (type == PLANE) {
    // Set up position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }
}

void GameObject::render(glm::vec3 pos, glm::mat4 projection, glm::mat4 view) {
  // Bind textures (if they exist)
  if (texture1) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
  }
  if (texture2) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
  }
  // Use shader
  shader->use();
  // Set uniforms
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);
  shader->setVec3("objectColor", 0.0f, 0.31f, 0.31f);
  shader->setVec3("lightColor",  1.0f, 1.0f, 1.0f);

  // Render
  glBindVertexArray(VAO);
  glm::mat4 model = glm::mat4(1.0f);
  // GameObject position
  model = glm::translate(model, pos);
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, verticesSize);
}
