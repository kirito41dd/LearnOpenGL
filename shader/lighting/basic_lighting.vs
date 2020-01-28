#version 330 core
layout (location = 0) in vec3 aPos;     // 顶点
layout (location = 1) in vec3 aNormal;  // 法线

out vec3 FragPos; // 片段
out vec3 Normal;  // 法线

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));    // 片段在世界坐标的位置
    Normal = mat3(transpose(inverse(model))) * aNormal;  // 把法线转换为世界坐标的方向，要构建法线矩阵，
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}