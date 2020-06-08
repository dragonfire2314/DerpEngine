#version 330 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 bone_ids;
layout (location = 4) in vec4 weights;

out vec2 TextureCoords;

void main(){
    gl_Position = vec4(vertPos, 1);

    TextureCoords = texCoord;
}