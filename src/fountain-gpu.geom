#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in Particle {
    vec2 pos;
    float alpha;
} particle[];

out Fragment {
    vec2 uv;
    float alpha;
} frag;

void main() {
    // Skip if not visible
    if (particle[0].alpha <= 0.0)
        return;

    float alpha = particle[0].alpha;

    vec4 center = vec4(particle[0].pos, 0.0, 1.0);

    // Emit four vertices to make a billboard
    vec2 uv = vec2(-1.0, -1.0) * 0.01;
    vec4 p = center;
    p.xy += uv;
    frag.uv = uv;
    frag.alpha = alpha;
    gl_Position = p;
    EmitVertex();

    uv = vec2(1.0, -1.0) * 0.01;
    p = center;
    p.xy += uv;
    frag.uv = uv;
    gl_Position = p;
    frag.alpha = alpha;
    EmitVertex();

    uv = vec2(-1.0, 1.0) * 0.01;
    p = center;
    p.xy += uv;
    frag.uv = uv;
    gl_Position = p;
    frag.alpha = alpha;
    EmitVertex();

    uv = vec2(1.0, 1.0) * 0.01;
    p = center;
    p.xy += uv;
    frag.uv = uv;
    frag.alpha = alpha;
    gl_Position = p;
    EmitVertex();

    EndPrimitive();

}