#version 330 core

out vec4 FragColor;

in vec2 FragCoords;

uniform vec2 resolution;
uniform float time;

vec3 rgb(float r, float g, float b) {
    return vec3(r / 255.0, g / 255.0, b / 255.0);
}

vec4 circle(vec2 uv, vec2 pos, float rad, vec3 color) {
    float d = length(pos - uv) - rad;
    float t = clamp(d, 0.0, 1.0);
    return vec4(color, 1.0 - t);
}

void main() {
    vec2 uv = FragCoords.xy;
    //vec2 center = resolution.xy * 0.5;
    vec2 center = vec2(0.31, 0.31) * resolution.xy;  // ???
    float radius = resolution.y * (1.5 - time);

    // background layer
    vec4 layer1 = vec4(rgb(210.0, 222.0, 228.0), 1.0);

    // circle
    vec3 black = rgb(0.0, 0.0, 0.0);
    vec4 layer2 = circle(uv, center, radius, black);

    // blend the two
    //FragColor = mix(layer1, layer2, layer2.a);
    FragColor = layer2;
}

