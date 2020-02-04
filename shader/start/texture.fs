#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;  // GLSL有一个供纹理对象使用的内建数据类型，叫做采样器(Sampler) 稍后我们会把纹理赋值给这个uniform

void main()
{
    FragColor = texture(ourTexture, TexCoord);
    // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0); // 顶点颜色和纹理颜色的混合色
}

// 你可能会奇怪为什么sampler2D变量是个uniform，我们却不用glUniform给它赋值。
// 使用glUniform1i，我们可以给纹理采样器分配一个位置值，这样的话我们能够在一个片段着色器中设置多个纹理。
// 一个纹理的位置值通常称为一个纹理单元(Texture Unit)。
// 一个纹理的默认纹理单元是0，它是默认的激活纹理单元，所以教程前面部分我们没有分配一个位置值。