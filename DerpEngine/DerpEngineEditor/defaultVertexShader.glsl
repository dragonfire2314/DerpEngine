#version 330 core

layout (location = 0) in vec3 vert;

uniform mat4 MVP;

void main(){
    gl_Position =  MVP * vec4(vert,1);
}