#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 crntPos;

void main() {
  Normal = aNormal;
  crntPos = vec3(model * vec4(aPos, 1.0f));

	gl_Position = projection * view * vec4(crntPos, 1.0);
}
