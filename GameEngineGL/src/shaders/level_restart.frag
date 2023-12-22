#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
//in vec3 Normal;
//in vec3 FragPos;

uniform vec2 resolution;
uniform float time;

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    //vec2 uv = 

    // vec4 blackAlpha = vec4(0.0, 0.0, 0.0, 1.0 * cos(time));
    // FragColor = blackAlpha;

    //vec2 uv = TexCoords / resolution.xy;

    //vec3 col = 0.5 + 0.5 * cos(time + uv.xyx + vec3(0, 2, 4));
    //vec3 col = 0.5 + 0.5 * cos(vec3(0, 2, 4));

    FragColor = vec4(0.0, 0.0, 0.0, abs(sin(time / 4.0)));
}
