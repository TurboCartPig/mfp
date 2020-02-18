#version 450

in Fragment {
    vec2 uv;
    float alpha;
} frag;

out vec4 FragColor;

void main() {
    FragColor = vec4(0.0, 1.0, 1.0, frag.alpha);
}
