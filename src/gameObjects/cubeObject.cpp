#include <cubeObject.h>

CubeObject::CubeObject(glm::vec3 pos) : pos(pos) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  verticesSize = sizeof(vertices);
  glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
  
  // Set up position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Set up texture attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

CubeObject::~CubeObject() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void CubeObject::render(glm::mat4 projection, glm::mat4 view) {
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
