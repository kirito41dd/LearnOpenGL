#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{             
    vec4 texColor = texture(texture1, TexCoords);
    if(texColor.a < 0.1)
        discard; // 如果透明 就直接丢弃这个片段  否则GL不知道如何处理这个颜色，在这个例子中透明部分会变成白色
    FragColor = texColor;
}