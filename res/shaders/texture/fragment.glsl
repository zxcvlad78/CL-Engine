#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 color;
uniform bool useTexture;

void main()
{
    if (useTexture)
    {
        FragColor = texture(texture1, TexCoord) * color;
    }
    else
    {
        FragColor = color;
    }
}