#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoords;

uniform vec3 LightPos;
uniform vec3 viewPos;
uniform sampler2D ourTexture;
uniform bool isDiffuse;

uniform vec3 diffuseColor;

layout (std140) uniform dirLight
{ 
    vec4 camera_position;
    vec4 light_position;
    vec4 light_diffuse;
};

void main(){

    //Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * light_diffuse.xyz;
    //Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_position.xyz - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_diffuse.xyz;   
    //Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camera_position.xyz - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_diffuse.xyz;  

    //Output
    vec3 result;
    if (isDiffuse) {
        color = vec4((ambient + diffuse + specular), 1) * texture(ourTexture, TextureCoords);
    }
    else {
        result = (ambient + diffuse + specular) * diffuseColor;
        color = vec4(result, 1.0);
    }
}