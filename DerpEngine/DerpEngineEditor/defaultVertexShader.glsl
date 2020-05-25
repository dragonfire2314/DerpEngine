#version 330 core

layout (location = 0) in vec3 vert;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
    gl_Position = Projection * View * vec4(vec3(Model * vec4(vert, 1.0)), 1.0);
}