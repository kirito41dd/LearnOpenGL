#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    // 透视除法是在顶点着色器运行之后执行的，将gl_Position的xyz坐标除以w分量 相除结果的z分量等于顶点的深度值
    // gl_Position = pos.xyww  那么最终的 z = w/w = 1.0  ，就是最大深度值 然后把深度测试设置为 小于等于就通过
}  