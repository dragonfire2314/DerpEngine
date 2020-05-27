#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoords;

uniform vec3 LightPos;
uniform vec3 viewPos;
uniform sampler2D ourTexture;

void main(){
    color = vec4(1, 0, 0, 1);
}