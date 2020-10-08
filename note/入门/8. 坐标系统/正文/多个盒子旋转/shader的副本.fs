#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1; // 采样器
uniform sampler2D texture2;

void main()
{
    // 输出FragColor 
//    FragColor = texture(ourTexture, TexCoord); // 纹理采样器 对应纹理坐标 // 纹理与颜色结合
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.8); // 0.2 表示 80%第一个输入 20%第二个输入
}
