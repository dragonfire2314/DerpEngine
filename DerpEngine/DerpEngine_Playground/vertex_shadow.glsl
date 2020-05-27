#version 330 core

layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TextureCoords;

void main(){

    FragPos = vec3(Model * vec4(vertexPosition_modelspace, 1.0));
    Normal = mat3(transpose(inverse(Model))) * aNormal;

    gl_Position = Projection * View * vec4(FragPos, 1.0);

    TextureCoords = texCoord;
}