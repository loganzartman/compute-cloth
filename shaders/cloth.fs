in vec4 vertex_pos;
in vec4 light_direction;
in vec3 gs_accel;
in vec3 gs_debug_color;
in vec3 barycentric;
in vec3 normal;

out vec4 frag_color;

void main() {
    vec3 real_normal = gl_FrontFacing ? normal : -normal;

    vec3 color = vec3(0.88,0.88,0.88);
    vec3 lighting = vec3(max(0, dot(light_direction.xyz, real_normal)));
    vec3 ambient = vec3(0.3,0.32,0.4);
    frag_color = vec4((ambient + lighting) * color, 1);
    frag_color = vec4(real_normal * 0.5 + 0.5, 1);

  //  frag_color = vec4(gs_debug_color, 1);
    if (barycentric.x < 0.1 && barycentric.y > 0.9)
        frag_color.b = 1;
}
