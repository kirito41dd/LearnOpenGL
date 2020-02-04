#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  


struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff; // 内锥圆
    float outerCutOff; // 外锥圆
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

// 模型数据
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;


void main()
{

    FragColor = texture(texture_diffuse1, TexCoords);
} 