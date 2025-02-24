#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMatrix;
layout (location = 7) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float scale;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragCol;

void main()
{
    mat4 mat = instanceMatrix;
    mat[0][0] *= scale;
    mat[1][1] *= scale;
    mat[3][0] *= scale;
    mat[3][1] *= scale;

    gl_Position = proj * view * mat * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(mat * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(mat))) * aNormal;
    FragCol = color;
}