in vec4 vertex_pos;
in vec4 light_direction;
flat in vec4 normal;

out vec4 frag_color;

void main() {
    vec3 color = vec3(0.88,0.88,0.88);
    vec3 lighting = vec3(max(0, dot(light_direction, normal)));
    vec3 ambient = vec3(0.3,0.32,0.4);
    frag_color = vec4((ambient + lighting) * color, 1);
}
