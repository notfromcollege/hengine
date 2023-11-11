#include <camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV), Gravity(GRAVITY) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
  float velocity = 0;
  if (Sprinting) velocity = (MovementSpeed*2) * deltaTime; else velocity = MovementSpeed * deltaTime;

  glm::vec3 localUp = glm::normalize(glm::vec3(0.0f, Up.y, 0.0f));
  glm::vec3 localFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
  glm::vec3 localRight = glm::normalize(glm::cross(localFront, Up));

  if (direction == UP) Position += localUp * velocity;
  if (direction == DOWN) Position -= localUp * velocity;

  if (direction == FORWARD && !MoveFly) Position += localFront * velocity;
  if (direction == FORWARD && MoveFly) Position += Front * velocity;

  if (direction == BACKWARD && !MoveFly) Position -= localFront * velocity;
  if (direction == BACKWARD && MoveFly) Position -= Front * velocity;

  if (direction == LEFT && !MoveFly) Position -= localRight * velocity;
  if (direction == LEFT && MoveFly) Position -= localRight * velocity;

  if (direction == RIGHT && !MoveFly) Position += localRight * velocity;
  if (direction == RIGHT && MoveFly) Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw   += xoffset;
  Pitch += yoffset;

  if (constrainPitch) {
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
  }

  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  // calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  // also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  Up    = glm::normalize(glm::cross(Right, Front));
}
