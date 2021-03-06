#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    // 输出FragColor 
    FragColor = texture(ourTexture, TexCoord); // 纹理采样器 对应纹理坐标
}
