#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoords;

uniform vec3 LightPos;
uniform vec3 viewPos;
uniform sampler2D ourTexture;

void main(){
/*
    vec3 dir = vec3(0,1,0);
    vec3 c = vec3(1,0,0);

    float diffuse = .2 + dot(Normal,dir);
    color = diffuse * c;
*/

    //color = vec3(TextureCoords.x, TextureCoords.y, 0);

    //color = vec4(dirLight, 1.0);

    //color = texture(ourTexture, TextureCoords);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos); 

    //Ambient
    vec3 objectColor = vec3(0.7, 0.5, 0.2);
    vec3 lightColor = vec3(1, 1, 1);

     //Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm); 

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor; 


    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0);

/*
    vec3 n1 = Normal + 1;
    n1 = n1 / 2;

	color = vec3(n1);
*/
}