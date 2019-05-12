in vec3 vs_vertex_pos;

out vec4 frag_color;

void main() {
    vec3 normal = normalize(vs_vertex_pos);
    float n_dot_l = max(dot(normal, vec3(0,1,0)), 0);
    frag_color = vec4(vec3(1,1,1) * n_dot_l + vec3(0.2), 1);
}
