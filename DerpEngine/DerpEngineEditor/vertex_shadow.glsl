#version 330 core

layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 bone_ids;
layout (location = 4) in vec4 weights;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

const int MAX_BONES = 100;

uniform bool isAnimated;
uniform mat4 bones[MAX_BONES];

out vec3 FragPos;
out vec3 Normal;
out vec2 TextureCoords;

void main(){

    if (isAnimated) {
        mat4 bone_transform = bones[int(bone_ids[0])] * weights[0];
		    bone_transform += bones[int(bone_ids[1])] * weights[1];
		    bone_transform += bones[int(bone_ids[2])] * weights[2];
		    bone_transform += bones[int(bone_ids[3])] * weights[3];
			
	    vec4 boned_position = bone_transform * vec4(vertexPosition_modelspace, 1.0); // transformed by bones

	    Normal = mat3(transpose(inverse(Model))) * aNormal;

	    FragPos = vec3(Model * boned_position);
	    TextureCoords = texCoord;

	    gl_Position = Projection * View * Model * boned_position;
    }
    else {
        FragPos = vec3(Model * vec4(vertexPosition_modelspace, 1.0));
        Normal = mat3(transpose(inverse(Model))) * aNormal;

        gl_Position = Projection * View * vec4(FragPos, 1.0);

        TextureCoords = texCoord;
    }
}