#version 330 core

layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 MVP;

out vec3 Normal;
out vec2 TextureCoords;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    vec4 temp = MVP * vec4(aNormal,0);
    Normal = temp.xyz;

    TextureCoords = texCoord;
}