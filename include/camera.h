#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
  UP,
  DOWN,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float FOV         =  90.0f;
const float GRAVITY     = -16.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

  glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

  void ProcessKeyboard(Camera_Movement direction, float deltaTime);  
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
  glm::vec3 getPos() { return Position; }

  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler Angles
  float Yaw;
  float Pitch;
  // camera options
  float MovementSpeed;
  bool Sprinting = false;
  float MouseSensitivity;
  float Fov;
  float Gravity;

private:
  void updateCameraVectors();
};
#endif
