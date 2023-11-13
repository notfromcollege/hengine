#version 330 core
struct Material {
  float ambient;
  float diffuse;
  float specular;
  float shininess;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 crntPos;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform Material material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
  // Ambient
  float ambient = material.ambient;

  // Diffuse
  vec3 normal = normalize(Normal);
  vec3 lightDir = normalize(lightPos - crntPos);
  float diffuse = max(dot(normal, lightDir), 0.0) + material.diffuse;

  // Specular
  float specularLight = 0.50;
  vec3 viewDir = normalize(camPos - crntPos);
  vec3 reflectionDir = reflect(-lightDir, normal);
  float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess);
  float specular = specAmount * (specularLight * material.specular);

  FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.4) * vec4(lightColor, 1.0) * (diffuse + ambient + specular);
}
