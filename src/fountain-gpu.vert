#version 450

layout(location = 0) in vec2 pos0;
layout(location = 1) in vec2 vel0;

out Particle {
    vec2 pos;
    float alpha;
};

uniform float time;
uniform vec2 acceleration;

void main() {
    float t = time;
    float lifetime = 2.0;
    pos = pos0 + vel0 * t + acceleration * (0.5 * t * t);
    float a = (t - 0.0) * (0.0 - 1.0) / (lifetime - 0.0) + 1.0;
    alpha = max(a, 0.0);
}