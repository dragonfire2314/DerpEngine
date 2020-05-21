#version 330 core
out vec4 color;

in vec3 Normal;
in vec2 TextureCoords;

uniform sampler2D ourTexture;

void main(){
/*
    vec3 dir = vec3(0,1,0);
    vec3 c = vec3(1,0,0);

    float diffuse = .2 + dot(Normal,dir);
    color = diffuse * c;
*/

    //color = vec3(TextureCoords.x, TextureCoords.y, 0);

    color = texture(ourTexture, TextureCoords);

/*
    vec3 n1 = Normal + 1;
    n1 = n1 / 2;

	color = vec3(n1);
*/
}