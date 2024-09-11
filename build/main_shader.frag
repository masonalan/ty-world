#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Flashlight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutoff;
    float outerCutoff;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform DirectionalLight dirLight;
uniform PointLight posLight;
uniform Flashlight flashlight;

void main()
{
    // Ambient light (there is always some baseline amount of light in a scene)
    //float ambientStrength = 0.1;
    vec3 color = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), 0.2).xyz;

    float distance    = length(posLight.position - FragPos);
    float attenuation = 1.0 / (posLight.constant + posLight.linear * distance + posLight.quadratic * (distance * distance));

    vec3 ambient = flashlight.ambient * color;
    //ambient *= attenuation;

    // Diffuse lightinge
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(-dirLight.direction);
    vec3 lightDir = normalize(flashlight.position - FragPos);

    float theta = dot(lightDir, normalize(-flashlight.direction));
    float epsilon   = flashlight.cutoff - flashlight.outerCutoff;
    float intensity = clamp((theta - flashlight.outerCutoff) / epsilon, 0.0, 1.0);

    //if (theta > flashlight.outerCutoff) {

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = flashlight.diffuse * (diff * color);
        //diffuse *= attenuation;
        diffuse *= intensity;

        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = flashlight.specular * (material.specular * spec);
        //specular *= attenuation;
        specular *= intensity;

        FragColor = vec4((ambient + diffuse + specular), 1.0);

//    } else {
//
//        FragColor = vec4(ambient * color, 1.0);
//    }
}