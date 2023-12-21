#version 330 core

out vec4 FragColor;

//in vec3 Normal;
//in vec3 FragPos;

uniform vec2 resolution;
uniform float time;

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    //vec2 uv = 

    vec4 blackAlpha = vec4(0.0, 0.0, 0.0, 1.0 * cos(time));
    FragColor = blackAlpha;
}
